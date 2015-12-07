#include "stdafx.h"
#include "GeoCalculation.h"
#include "Vector2dFactory.h"
#include "AngleUtility.h"
#include "GraphUtility.h"

Geography::GeoCalculation::GeoCalculation()
{
}


Geography::GeoCalculation::~GeoCalculation()
{
}


///<summary>
/// Harversine���v�Z����
/// �Q��(http://www.movable-type.co.uk/scripts/latlong.html)
///</summary>
double Geography::GeoCalculation::haversine(const Geography::LatLng& ll1, const Geography::LatLng& ll2)
{
	double lat1 = Math::AngleUtility::convert_to_radian(ll1.lat());
	double lat2 = Math::AngleUtility::convert_to_radian(ll2.lat());
	double d_lat = Math::AngleUtility::convert_to_radian(ll2.lat() - ll1.lat());
	double d_lng = Math::AngleUtility::convert_to_radian(ll2.lng() - ll1.lng());
	return std::pow(std::sin(d_lat / 2.0), 2) + std::cos(lat1) * std::cos(lat2) * std::pow(std::sin(d_lng / 2.0), 2);
}


///<summary>
/// Haversine�@�Ɋ�Â������v�Z (m)
/// �Q��(http://www.movable-type.co.uk/scripts/latlong.html)
///</summary>
double Geography::GeoCalculation::haversine_distance(const Geography::LatLng& ll1, const Geography::LatLng& ll2)
{
	double a = haversine(ll1, ll2);
	double c = 2.0 * std::atan(std::sqrt(a) / std::sqrt(1 - a));
	return R * c;
}


///<summary>
/// Lambert-Andoyer�@�Ɋ�Â������v�Z(m)�C���x��haversine��荂�������x�������
/// �Q��(http://www2.nc-toyama.ac.jp/~mkawai/lecture/sailing/geodetic/geosail.html)
///</summary>
double Geography::GeoCalculation::lambert_distance(const Geography::LatLng& ll1, const Geography::LatLng& ll2)
{

	if (ll1 == ll2) return 0.0;

	//�G�����Ƌɔ��a
	static const double F = 1.0 / 298.257222101;

	//���W�A���P�ʂɕϊ�
	double lat1 = Math::AngleUtility::convert_to_radian(ll1.lat());
	double lat2 = Math::AngleUtility::convert_to_radian(ll2.lat());
	double lng1 = Math::AngleUtility::convert_to_radian(ll1.lng());
	double lng2 = Math::AngleUtility::convert_to_radian(ll2.lng());

	//�����ܓx�̌v�Z
	double phi1 = std::atan( (1.0 - F) * std::tan(lat1) );
	double phi2 = std::atan( (1.0 - F) * std::tan(lat2) );

	//���ʏ�̋���
	double spherical_distance = std::acos(std::sin(phi1) * std::sin(phi2) + std::cos(phi1) * std::cos(phi2) * std::cos(lng1 - lng2));

	//Lambert-Andoyer�̏C����
	double s = (std::sin(spherical_distance) - spherical_distance) * std::pow(std::sin(phi1) + std::sin(phi2), 2) / std::pow(std::cos(spherical_distance / 2), 2);
	double c = (std::sin(spherical_distance) + spherical_distance) * std::pow(std::sin(phi1) - std::sin(phi2), 2) / std::pow(std::sin(spherical_distance / 2), 2);
	double d_rho = (F / 8) * (c - s);

	//�C����̋���
	return R * (spherical_distance + d_rho);
}



///<summary>
/// Lambert-Andoyer�@�Ɋ�Â����ʊp�̌v�Z
/// from����to�Ɍ����������̕��ʊp���v�Z����D
/// (��0�x�C��90�x�C��180�x�C�k270�x�C�P�ʂ̓��W�A��)
/// from��to���������ꍇ��-1.0���o��
/// �Q��(http://www2.nc-toyama.ac.jp/~mkawai/lecture/sailing/geodetic/geosail.html)
///</summary>
double Geography::GeoCalculation::lambert_azimuth_angle(const Geography::LatLng& from, const Geography::LatLng& to)
{
	//�G�����Ƌɔ��a
	static const double F = 1.0 / 298.257222101;

	//���W�A���P�ʂɕϊ�
	double lat1 = Math::AngleUtility::convert_to_radian(from.lat());
	double lat2 = Math::AngleUtility::convert_to_radian(to.lat());
	double lng1 = Math::AngleUtility::convert_to_radian(from.lng());
	double lng2 = Math::AngleUtility::convert_to_radian(to.lng());

	//�����ܓx�̌v�Z
	double phi1 = std::atan((1.0 - F) * std::tan(lat1));
	double phi2 = std::atan((1.0 - F) * std::tan(lat2));

	//h��Zc�̌v�Z
	double h = lng1 - lng2;
	if (h < 0) h += 2.0 * M_PI;
	double Zc = std::atan(std::sin(h) / (std::cos(phi1) * std::tan(phi2) - std::sin(phi1) * std::cos(h)));

	//��0�x�̎��v����̍��W�n�ɕϊ�
	if (h <= M_PI) {
		if (Zc >= 0) {
			Zc = 3.0 * M_PI_2 - Zc;
		}
		else {
			Zc = M_PI_2 - Zc;
		}
	}
	else {
		if (Zc >= 0) {
			Zc = M_PI_2 - Zc;
		}
		else {
			Zc = 3.0 * M_PI_2 - Zc;
		}
	}
	return Zc;
}


///<summary>
/// �񃁃��o�֐�
/// LatLng�̃��X�g��擪�̗v�f����Ƃ����������W�n�ɕϊ�����
/// ���W�n�͉E�������̕���
/// point_list����̏ꍇ�͋��vector���Ԃ���܂�
///</summary>
std::vector<std::shared_ptr<Graph::Coordinate const>> convert_to_cartesian(const std::vector<std::shared_ptr<Geography::LatLng const>>& point_list)
{
	std::vector<std::shared_ptr<Graph::Coordinate const>> ret;

	std::vector<std::shared_ptr<Geography::LatLng const>>::const_iterator iter = point_list.begin();
	std::shared_ptr<Geography::LatLng const> reference_point = *iter;
	
	if (iter != point_list.end()) {
		iter++;
		ret.push_back(std::make_shared<Graph::Coordinate const>(0U, 0.0, 0.0));
	}

	unsigned int id = 1;
	for (; iter != point_list.end(); iter++) {
		if (*reference_point == **iter) {
			ret.push_back(std::make_shared<Graph::Coordinate const>(0U, 0.0, 0.0));
			continue;
		}
		double distance = Geography::dist(*reference_point, **iter);
		double azimuth_angle = Geography::angle(*reference_point, **iter);
		Graph::Vector2d position_vector = Graph::Vector2dFactory::create_by_polar(distance, azimuth_angle);
		ret.push_back(std::make_shared<Graph::Coordinate const>(id++, position_vector.x(), position_vector.y()));
	}

	return ret;
}

///<summary>
/// �ʕ�ɊY������LatLng��vector���擾����
/// point_list����̏ꍇ�͋��vector���Ԃ���܂�
///</summary>
const std::vector<std::shared_ptr<Geography::LatLng const>> Geography::GeoCalculation::convex_hull(const std::vector<std::shared_ptr<Geography::LatLng const>>& point_list)
{

	std::vector<std::shared_ptr<Geography::LatLng const>> ret;

	std::vector<std::shared_ptr<Graph::Coordinate const>> cartesian_points = convert_to_cartesian(point_list);
	if (cartesian_points.size() != 0) {
		const std::vector<std::shared_ptr<Graph::Coordinate const>> convex_points = Graph::GraphUtility::convex_hull(cartesian_points);
		for (std::vector<std::shared_ptr<Graph::Coordinate const>>::const_iterator iter = convex_points.begin(); iter != convex_points.end(); iter++) {
			ret.push_back(point_list.at((*iter)->get_id()));
		}
	}
	return ret;
}


///<summary>
/// �^����ꂽ�n�_���X�g�̈ܓx�C�o�x����ɓʕ�̖ʐ�[m^2]���v�Z���܂�
///</summary>
double Geography::GeoCalculation::calc_convex_hull_size(const std::vector<std::shared_ptr<Geography::LatLng const>> &point_list)
{
	std::vector<std::shared_ptr<Graph::Coordinate const>> cartesian_points = convert_to_cartesian(point_list);
	return Graph::GraphUtility::calc_convex_hull_size(cartesian_points);
}


const double Geography::GeoCalculation::B = 6356752.31414036;
const double Geography::GeoCalculation::F = 0.0033528106811823;

///<summary>
/// from���w��̋����ƕ��ʊp(rad)�ňړ������_���v�Z����
/// ���ʊp�͓����玞�v����̍��W�n
/// �v�Z�͂����炭Lambert�@�ł͂Ȃ��C���L�ŗ��p���Ă�����̂�o�p�����D
/// http://www.mapli.net/direct/
///</summary>
Geography::LatLng Geography::GeoCalculation::calc_translated_point(const LatLng& from, double distance, double azimuth_angle)
{
	const int ITERATION_MAX = 5;

	azimuth_angle += M_PI_2;
	if (azimuth_angle < 0) azimuth_angle += 2 * M_PI;
	if (azimuth_angle > 2 * M_PI) azimuth_angle -= 2 * M_PI;

	double latitude = Math::AngleUtility::convert_to_radian(from.lat());
	double longitude = Math::AngleUtility::convert_to_radian(from.lng());

	double sin = std::sin(azimuth_angle);
	double cos = std::cos(azimuth_angle);

	double u1 = std::atan((1.0 - F) * std::tan(latitude));
	double sigma1 = std::atan2((1.0 - F) * std::tan(latitude), cos);

	double sin_u1 = std::sin(u1);
	double cos_u1 = std::cos(u1);

	double sin_alpha = cos_u1 * sin;
	double cos_alpha2 = 1 - sin_alpha * sin_alpha;

	double uu = cos_alpha2 * (R * R - B * B) / (B * B);
	double a = 1 + (uu / 16384.0) * (4096.0 + uu * (-768.0 + uu * (320.0 - 175.0 * uu)));
	double b = (uu / 1024.0) * (256.0 + uu * (-128.0 + uu * (74.0 - 47.0 * uu)));

	double sigma = distance / (B * a);
	double sigma0, sin_sigma, cos_sigma, cos2_sigma_m, d_sigma;
	for (int i = 0; i < ITERATION_MAX; i++) {
		cos2_sigma_m = std::cos(2 * sigma1 + sigma);
		sin_sigma = std::sin(sigma);
		cos_sigma = std::cos(sigma);
		d_sigma = b * sin_sigma * (cos2_sigma_m + (b / 4.0) * (cos_sigma * (-1.0 + 2.0 * std::pow(cos2_sigma_m, 2))
			- (b / 6.0) * cos2_sigma_m * (-3.0 + 4.0 * std::pow(sin_sigma, 2)) * (-3.0 + 4.0 * std::pow(cos2_sigma_m, 2))));
		sigma0 = sigma;
		sigma = distance / (B * a) + d_sigma;

		if (std::abs(sigma - sigma0) <= 1.0E-12) break;
	}

	cos2_sigma_m = std::cos(2.0 * sigma1 + sigma);
	sin_sigma = std::sin(sigma);
	cos_sigma = std::cos(sigma);
	double phi2 = std::atan2(sin_u1 * cos_sigma + cos_u1* sin_sigma * cos, (1 - F) * std::sqrt(sin_alpha * sin_alpha + std::pow(sin_u1 * sin_sigma - cos_u1 * cos_sigma * cos, 2)));
	double lambda = std::atan2(sin_sigma * sin, cos_u1 * cos_sigma - sin_u1 * sin_sigma * cos);
	double c = (F / 16.0) * cos_alpha2 * (4.0 + F * (4.0 - 3.0 * cos_alpha2));

	double omega = lambda - (1.0 - c) * F * sin_alpha * (sigma + c * sin_sigma * (cos2_sigma_m + c * cos_sigma * (-1.0 + 2.0 * std::pow(cos2_sigma_m, 2))));
	double lambda2 = longitude + omega;

	return Geography::LatLng(Math::AngleUtility::convert_to_degree(phi2), Math::AngleUtility::convert_to_degree(lambda2));
}