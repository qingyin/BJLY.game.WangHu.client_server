
#include "MCWebReq.h"
#include "Game/Script/utility.h"
#include "Game/Script/ScriptData.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"

using namespace std;


MCWebReq::MCWebReq()
{
}

 void MCWebReq::init()
 {
	 static ScriptData<std::string> s_kTxt("WebIP");
	 static ScriptData<float> s_fTimeOut("HttpTimeOut");
	m_kWebIP = s_kTxt.Value();
	m_kSocketIP = "";
	cocos2d::network::HttpClientEx::getInstance()->setTimeoutForConnect(s_fTimeOut.Value());
	//std::string kUrl = "data_interface/client_config.aspx";
	//MCWebReq::instance().sendRequestDocument(kUrl,CC_CALLBACK_1(MCWebReq::CB_IP,this));
}
 std::string MCWebReq::getSocketIP()
 {
	 return m_kSocketIP;
 }
 std::string MCWebReq::getWebIp()
 {
	 return m_kWebIP+"/";
 }
 void MCWebReq::CB_IP(rapidjson::Document* pDoc)
 {
	 m_kWebIP = MCWebReq::getDataValueStr(pDoc,"web_service_root");
	 m_kSocketIP = MCWebReq::getDataValueStr(pDoc,"logon_server_ip");
 }
 void MCWebReq::sendRequestStr(std::string kUrl
	 ,std::function<void(std::string)> _callBack1)
 {
	 kUrl = utility::toString(getWebIp(),kUrl);
	 sendRequestDocumentUrl(kUrl,nullptr,_callBack1);
 }
 void MCWebReq::sendRequestDocument(std::string kUrl
	 ,std::function<void(rapidjson::Document*)> _callBack1)
 {
	 kUrl = utility::toString(getWebIp(),kUrl);
	 sendRequestDocumentUrl(kUrl,_callBack1,nullptr);
 }
 void MCWebReq::sendRequestDocumentUrl(std::string kUrl
	 ,std::function<void(rapidjson::Document*)> _callBack1
	 ,std::function<void(std::string)> _callBack2)
 {
	 cocos2d::network::HttpRequestEx *request = new cocos2d::network::HttpRequestEx();
	 request->setUrl(kUrl.c_str());
	 request->setRequestType(cocos2d::network::HttpRequestEx::Type::POST);

	 utility::log("web url:%s",request->getUrl());
	 request->setResponseCallback(CC_CALLBACK_2(MCWebReq::resCallBack,this));
	 cocos2d::network::HttpClientEx::getInstance()->sendImmediate(request);
	 request->release();
	 SendStructInfo kInfo;
	 kInfo.m_fJsonResCallBack = _callBack1;
	 kInfo.m_fStrResCallBack = _callBack2;
	 kInfo.pRequest = request;
	 m_kCBList.push_back(kInfo);
 }

 std::string MCWebReq::getPostData( std::map<std::string,std::string>& paramenters )
 {
	 return  joinMapToData(paramenters);
 }

 std::string MCWebReq::joinMapToData( std::map<std::string, std::string> &paramenters )
 {
	 string data = "";
	 for(map<string,string>::iterator it = paramenters.begin(); it != paramenters.end(); ++it) {
		 if(it != paramenters.begin()){
			 data += "&";
		 }
		 data += it->first + "=" + it->second;
	 }
	 return data;
 }
 void MCWebReq::resCallBack( cocos2d::network::HttpClientEx* client,cocos2d::network::HttpResponseEx* response )
 {
	 SendStructInfo kInfo;
	 for (SendStructInfoList::iterator itor = m_kCBList.begin();itor != m_kCBList.end();itor++)
	 {
		 SendStructInfo& kTemp = *itor;
		 if (kTemp.pRequest == response->getHttpRequest())
		 {
			 kInfo = kTemp;
			 m_kCBList.erase(itor);
			 break;
		 }
	 }
	 CCAssert(kInfo.pRequest,"");

	 if(kInfo.m_fStrResCallBack)
	 {
		kInfo.m_fStrResCallBack(parseResData(response));
	 }
	 if (kInfo.m_fJsonResCallBack) 
	 {
		 std::string kData =  parseResData(response);
		 std::string::iterator itor = kData.begin();
		 rapidjson::Document doc;
		 if (parseDataByJson(kData,doc))
		 {
			 kInfo.m_fJsonResCallBack(&doc);
		 }
	 }
 }
 void MCWebReq::pushValue(std::string& kUrl,std::string kKey,std::string kValue)
 {
	 if (kUrl.back() != '?')
	 {
		 kUrl += "&";
	 }
	 kUrl += kKey;
	 kUrl += "=";
	 kUrl += kValue;
 }

 bool MCWebReq::parseResByJson( cocos2d::network::HttpResponseEx *response,rapidjson::Document &doc )
 {
	 std::string _data =  parseResData(response);
	 if(_data==""||_data.size()<=0)
		 return false;
	 return  parseDataByJson(_data,doc);
 }

 std::string MCWebReq::parseResData( cocos2d::network::HttpResponseEx *response )
 {
	 if(!(response->isSucceed())){
		 const char *error = response->getErrorBuffer();
		 utility::log("error:%s, code:%ld", error, response->getResponseCode());
		 utility::log("XQ5WebReq::getParseResData error = %s",error);
		 return "";
	 }
	 std::vector<char> *buffer = response->getResponseData();

	 std::string str;
	 for (unsigned int i = 0; i < buffer->size(); i++){
		 str.push_back((*buffer)[i]);
	 }
	 utility::log("XQ5WebReq::getParseResData Sucess = %s",str.c_str());
	 return str;
 }

 bool MCWebReq::parseDataByJson( const std::string& data, rapidjson::Document &doc )
 {
	 if(data == "")
	 {
		 return false;
	 }
	 doc.Parse<0>(data.c_str());
	 if (doc.GetParseError())
	 {
		 utility::log(utility::toString(doc.GetParseError()).c_str());
		 return false;
	 }
	 return true;
 }


 void MCWebReq::sendFile(std::string kUrl, std::string &fileName,
	 const cocos2d::network::ccHttpRequestCallback &callback)
 {
	 kUrl = utility::toString("http://",getWebIp(),"/"+kUrl);
	 CCAssert(fileName != "","");
	 cocos2d::network::HttpRequestEx *request = new cocos2d::network::HttpRequestEx();
	 request->setUrl(kUrl.c_str());

	 request->setResponseCallback(callback);

	 request->setRequestType(cocos2d::network::HttpRequestEx::Type::PostFile);
	 request->setFile(fileName.c_str(), "image/jpeg", "img");

	 cocos2d::network::HttpClientEx::getInstance()->sendImmediate(request);
	 request->release();
 } 
 void MCWebReq::sendFileURLHeadImgaic(std::string kUrl, std::string &fileName,
	 std::function<void(rapidjson::Document*)> _callBack)
 {
	 kUrl = utility::toString("http://",getWebIp(),"/"+kUrl);
	 cocos2d::network::HttpRequestEx *request = new cocos2d::network::HttpRequestEx();
	 request->setUrl(kUrl.c_str());

	 request->setResponseCallback(CC_CALLBACK_2(MCWebReq::resCallBack,this));

	 request->setRequestType(cocos2d::network::HttpRequestEx::Type::PostFile);
	 request->setFile(fileName.c_str(), "image/jpeg", "img");

	 cocos2d::network::HttpClientEx::getInstance()->sendImmediate(request);
	 request->release();


	 SendStructInfo kInfo;
	 kInfo.m_fJsonResCallBack = _callBack;
	 kInfo.pRequest = request;
	 m_kCBList.push_back(kInfo);
 }
 cocos2d::network::HttpRequestEx* MCWebReq::getFile(std::string kUrl,const cocos2d::network::ccHttpRequestCallback &callback)
 {
	 kUrl = utility::toString("http://",getWebIp(),"/"+kUrl);

	 cocos2d::network::HttpRequestEx *request = new cocos2d::network::HttpRequestEx();
	 request->setUrl(kUrl.c_str());

	 request->setResponseCallback(callback);

	 request->setRequestType(cocos2d::network::HttpRequestEx::Type::GET);

	 cocos2d::network::HttpClientEx::getInstance()->sendImmediate(request);
	 request->release();
	 return request;
 }

 cocos2d::network::HttpRequestEx* MCWebReq::getFileEx( std::string kUrl,const cocos2d::network::ccHttpRequestCallback &callback )
 {
	 cocos2d::network::HttpRequestEx *request = new cocos2d::network::HttpRequestEx();
	 request->setUrl(kUrl.c_str());

	 request->setResponseCallback(callback);

	 request->setRequestType(cocos2d::network::HttpRequestEx::Type::GET);

	 cocos2d::network::HttpClientEx::getInstance()->sendImmediate(request);
	 request->release();
	 return request;
 }
