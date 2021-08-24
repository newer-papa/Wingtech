#pragma once
#include "ui_ParameterSetting.h"
#include "ImageCapture.h"
#include "QButtonGroup"
#include "QFileDialog"
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


class CParameterSetting :public QDialog
{
	Q_OBJECT
public:
	CParameterSetting(QDialog *parent = NULL);
	~CParameterSetting();
	void StartDetecion(bool bStart);
private:
	void InitVariables();
	bool CloseDevice(int index);
	void InitConnections();
	void InitCamera();
	void SetButtonGroupEnabled(bool enabled, int index);
	bool OpenCamera(MV_CC_DEVICE_INFO device_info, int index);
	QImage MattoQImage(Mat image);

    int GetExposureTime(const CMvCamera &CameraHandle);  // ch:设置曝光时间 | en:Set Exposure Time
    int SetExposureTime(const CMvCamera &CameraHandle);
    int GetGain(const CMvCamera &CameraHandle);  // ch:设置增益 | en:Set Gain
    int SetGain(const CMvCamera &CameraHandle);

	void ShowErrorMsg(QString Message, int nErrorNum) ;
    void getCameraParams();

private slots:
	//void ReceivaAlgoImage( Mat OriginalImage, e_CameraType type, int Time,bool bok);
	void OpenFirstCamera();
	void ReceivaAlgoImage(Mat OriginalImage, e_CameraType type, int Time, bool bok);
	void LoadFirstImage();
	void SwitchFirstCameraStatus(int index, bool checked);
    void SaveCameraParams1();
signals:
	void SendAlgoImageToMainWindow( Mat OriginalImage, e_CameraType type,int index, bool bok);
private:
	Ui::DialogSetting ui;
	s_CameraInfo m_FirstCameraInfo;
	s_CameraInfo m_SecondCameraInfo;
	s_CameraInfo m_ThirdCameraInfo;
	s_CameraInfo m_FourCameraInfo;
	QButtonGroup *m_FirstGroup;
	MV_CC_DEVICE_INFO_LIST m_stDevList;
	bool m_bFirstAlgoSuccess;
	Mat m_FirstOriginalImage;
	Mat m_FirstRenderImage;

    // ch:曝光时间 | en:Exposure Time
    double m_dExposureEdit{0.0};
    // ch:增益 | en:Gain
    double m_dGainEdit{0.0};
    // ch:帧率 | en:Frame Rate
    double m_dFrameRateEdit{0.0};

};
