
## Simulator for Location Privacy Team (written by Seiya Mizuno)

###初めて使う人へ
**最初に一度管理者権限でVisual Studioを起動し，Release構成でビルドしましょう．**

###プロジェクト構成
* LocationPrivacy: 各種提案手法に特化した実装をしているコードを格納するプロジェクト
* LocationPrivacyTest: LocationPrivacyのテストプロジェクト
* Utils: どの手法でも使えるコードを集めたプロジェクト
* UtilsTest: Utilsのテストプロジェクト

###ビルド構成について
外部ライブラリとしてMySQL Connector C++を使用しています．(ソースからビルドする必要があったので苦労しました)  
そのためlibmysql.dllの32bit版をC:/Windowsに置く必要があります．  
libmysqlもプロジェクト内に含めてあり，Release構成で実行すると，自動的にビルド前イベントで
C:/windowsに配置されます．  
**Releaseビルドは管理者権限でVisual Studioを起動していないと通らないので注意!**  

###MySQL Connector C++をソースからビルドする (おまけ)

1. [CMake](http://www.cmake.org/download/)をインストールする．
2. [MySQL Connector C](http://dev.mysql.com/downloads/connector/c/)をインストール (ビルド済みの方)
3. [MySQL Connector C++](http://dev.mysql.com/downloads/connector/cpp/)のソースファイルをダウンロード(ドロップダウンからSource Codeを選択)
4. [Boost](http://sourceforge.net/projects/boost/files/boost/1.59.0/)をダウンロードして2のソースと同じディレクトリに置く
5. C:/直下にMySQLというディレクトリを作成し，2でダウンロードしたMySQL Connector Cを置く
6. MySQL Connector C++のソースのディレクトリに移動し次のコマンドを入力，これを実行するとソリューションができる．  
	```cmake -G "Visual Studio 14 2015" -DBOOST_ROOT:STRING=../boost_1_59_0```
7. 4で作成したソリューションをVisual Studioで開く
8. mysql_private_iface.hからsnprintfをdefineしている部分を消す
9. mysqlcppconn-staticプロジェクトをビルド
