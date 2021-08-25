#pragma once
#include "ui_ParameterSetting.h"
#include "ImageCapture.h"
#include "QButtonGroup"
#include "QFileDialog"
#include "PLCManager.h"
#include "Algo.h"
#include "SaveImage.h"
#include "QFileDialog"
#include "QDateTime"
#include "qtablewidget.h"
#include "QHeaderView"
#include "Config.h"
typedef struct
{
	QString CameraName;
	CMvCamera CameraHandle;
	bool bOpenCamera;
	CImageCapture *ImageCapture;
	void Init()
	{
		CameraName.clear();
		bOpenCamera = false;
		ImageCapture = new CImageCapture();

	}
}s_CameraInfo;

typedef struct
{
	QString CameraName;
	CMvCamera CameraHandle;
	bool bOpenCamera;
	CAlgo *Algo;
	void Init()
	{
		Algo = new CAlgo();

	}
}s_AlgoInfo;

typedef struct
{
	Mat OriginalImage;
	Mat RenderImage;
	bool bok;
}s_StationInfo;

typedef struct
{
	s_StationInfo FirstStation;
	s_StationInfo SecondStation;
	s_StationInfo ThirdStation;
	s_StationInfo FourStation;
}s_SaveImageInfo;

class CParameterSetting :public QDialog
{
	Q_OBJECT
public:
	CParameterSetting(QDialog *parent = NULL);
	~CParameterSetting();
	void StartDetecion(bool bStart);
private:
	void InitVariables();
	void InitTableWidget();
	bool CloseDevice(int index);
	void InitConnections();
	void InitCamera();
	void SetButtonGroupEnabled(bool enabled, int index);
	bool OpenCamera(MV_CC_DEVICE_INFO device_info, int index);
	QImage MattoQImage(Mat image);
	void LoadConfig();/////////////////////
    int GetExposureTime(CMvCamera &CameraHandle, int index);  // ch:�����ع�ʱ�� | en:Set Exposure Time
    int SetExposureTime(CMvCamera &CameraHandle, int index);
    int GetGain(CMvCamera &CameraHandle, int index);  // ch:�������� | en:Set Gain
    int SetGain(CMvCamera &CameraHandle, int index);


	void ShowErrorMsg(QString Message, int nErrorNum) ;
       void getCameraParams(int index);

	void InitFirstGroup();
	void InitSecondGroup();
	void InitThirdtGroup();
	void InitFourthGroup();

	void SafeParamsSetting();
	void SaveImage(s_SaveImageInfo ImageInfo);
private slots:
	void OpenFirstCamera();
	void LoadFirstImage();
	void SwitchFirstCameraStatus(int index, bool checked);
    void SaveCameraParams1();
	void SaveCameraParams2();
    void SaveCameraParams3();
    void SaveCameraParams4();
	void SaveConfig();////////
	void OpenSecondCamera();
	void LoadSecondImage();
	void SwitchSecondCameraStatus(int index, bool checked);
	void OpenThirdCamera();
	void SwitchThirdCameraStatus(int index, bool checked);
	void OpenFourthCamera();
	void ReceivaAlgoImage(Mat OriginalImage, e_CameraType type, int Time, bool bok);
	void SwitchFourthCameraStatus(int index, bool checked);
	void ConnectToPLC();
	void SendOKToPLC();
	void SendNGToPLC();
	void SetSystemType(int index);

	void ReceiveCameraImage(Mat image, int index);
     
signals:
	void SendAlgoImageToMainWindow( Mat OriginalImage, e_CameraType type,int index, bool bok);
	//void SendImageToAlgo(Mat OriginalImage, e_CameraType type, int index, bool bok);
private:
	Ui::DialogSetting ui;
	s_CameraInfo m_FirstCameraInfo;
	s_CameraInfo m_SecondCameraInfo;
	s_CameraInfo m_ThirdCameraInfo;
	s_CameraInfo m_FourCameraInfo;
	QButtonGroup *m_FirstGroup;
	QButtonGroup *m_SecondGroup;
	QButtonGroup *m_ThirdGroup;
	QButtonGroup *m_FourthGroup;
	MV_CC_DEVICE_INFO_LIST m_stDevList;
	bool m_bFirstAlgoSuccess;
	Mat m_FirstOriginalImage;
	Mat m_FirstRenderImage;

	Mat m_SecondOriginalImage;
	Mat m_SecondRenderImage;
	s_AlgoInfo m_FirstAlgo;
	CSaveImage m_SaveImage;
    // ch:�ع�ʱ�� | en:Exposure Time
    double m_dExposureEdit{0.0};
    // ch:���� | en:Gain
    double m_dGainEdit{0.0};
    // ch:֡�� | en:Frame Rate
    double m_dFrameRateEdit{0.0};


	QTableWidget m_ModelChoice;

};
