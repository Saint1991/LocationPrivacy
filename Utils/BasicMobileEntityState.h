#pragma once
#include "FileExportable.h"
#include "Coordinate.h"
#include "LatLng.h"

namespace Entity 
{

	///<summary>
	/// ���[�U�C�y�у_�~�[�̏�Ԃ��L�q����f�[�^�\��
	/// ���W�n�ɂ����POSITION_TYPE���g��������(Coordinate or LatLng)
	///</summary>
	template <typename POSITION_TYPE>
	class BasicMobileEntityState : public IO::FileExportable
	{
	
	//Coordinate�̔h���N���X���`�F�b�N
	static_assert(std::is_base_of<Graph::Coordinate, POSITION_TYPE>::value, "template type POSITION_TYPE must be derived from Coordinate");

	protected:
		static const std::string USER_PROBABILITY;

		std::shared_ptr<POSITION_TYPE> position;
		double user_probability;

	public:
		BasicMobileEntityState(const POSITION_TYPE& position, double probability);
		virtual ~BasicMobileEntityState();

		double get_user_probability() const;
		const std::shared_ptr<POSITION_TYPE const> read_position() const;

		std::unordered_map<std::string, std::string> get_export_data() const;
	};

	//�����I���ꉻ
	template class BasicMobileEntityState<Graph::Coordinate>;
	template class BasicMobileEntityState<Geography::LatLng>;
}


