#include "ParameterSetting.h"
#include "QMessageBox"
CParameterSetting::CParameterSetting(QDialog *parent /*= NULL*/)
	:QDialog(parent)
{
	ui.setupUi(this);
	InitVariables();
	InitConnections();
	InitCamera();
}

CParameterSetting::~CParameterSetting()
{
	qDebug() << "~CParameterSetting";
	m_FirstCameraInfo.ImageCapture->StopThread();
	m_FirstCameraInfo.ImageCapture->wait(1000);
	if (m_FirstCameraInfo.bOpenCamera)
	{
		CloseDevice(1);
	}
	qDebug() << "~CParameterSetting";
}

void CParameterSetting::InitVariables()
{
	//初始化
	m_FirstCameraInfo.Init();
	//m_SecondCameraInfo.Init();
	//m_ThirdCameraInfo.Init();
	//m_FourCameraInfo.Init();

	//开启线程
	m_FirstCameraInfo.ImageCapture->start();
	//m_SecondCameraInfo.ImageCapture->start();
	//m_ThirdCameraInfo.ImageCapture->start();
	//m_FourCameraInfo.ImageCapture->start();

	m_FirstGroup = new QButtonGroup();
	m_FirstGroup->addButton(ui.radioButton_FreeFirst, 1);
	m_FirstGroup->addButton(ui.radioButton_ExternalFirst, 2);
	m_FirstGroup->addButton(ui.radioButton_SoftFirst, 3);
	SetButtonGroupEnabled(false, 1);

	m_FirstCameraInfo.ImageCapture->SetSystemType(CAMERA_FIRST);
	QString errMsg;
	//m_bFirstAlgoSuccess = CAlgoFirstStation::GetInstance()->InitAlgo(errMsg); //初始化模型

}

bool CParameterSetting::CloseDevice(int index)
{
	m_FirstCameraInfo.ImageCapture->SetCameraStatus(false);
	int nRet = m_FirstCameraInfo.CameraHandle.StopGrabbing();
	if (nRet != MV_OK)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("错误"), "Frist Camera StopGrabbing failed:" + QString::number(nRet));
		return false;
	}
	nRet = m_FirstCameraInfo.CameraHandle.Close();
	if (nRet == MV_OK)
	{
		m_FirstCameraInfo.bOpenCamera = false;
		return true;
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("错误"), "close first device failed:" + QString::number(nRet));
		return false;
	}
}

void CParameterSetting::InitConnections()
{
	qRegisterMetaType<Mat>("Mat");
	qRegisterMetaType<e_CameraType>("e_CameraType");
	connect(m_FirstCameraInfo.ImageCapture, SIGNAL(SendAlgoImage(Mat, e_CameraType,int,bool)), this, SLOT(ReceivaAlgoImage( Mat, e_CameraType,int,bool)));
	connect(m_FirstGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(SwitchFirstCameraStatus(int, bool)));
    //connect(ui.pushButton_SaveParams, SIGNAL(clicked()), this, SLOT(SaveCameraParams1()));
}

void CParameterSetting::InitCamera()
{
	qDebug() << "InitCameraInfos";
	printf("InitCameraInfos\n");
	memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
	int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE, &m_stDevList);
	if (MV_OK != nRet)
	{
		qDebug() << QString::fromLocal8Bit("枚举相机失败:") << nRet;
		printf("枚举相机失败\n");
		return;
	}
	ui.pushButton_OpenFirst->setEnabled(m_stDevList.nDeviceNum > 0);
	for (int i = 0; i < m_stDevList.nDeviceNum; ++i)
	{
		MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
		char strUserName[256] = { 0 };
		sprintf_s(strUserName, "%s-%s_%s", pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName, pDeviceInfo->SpecialInfo.stGigEInfo.chModelName,
			pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
		QString UserName = QString::fromLocal8Bit(strUserName);
		ui.comboBox_First->addItem(UserName);
		qDebug() << "CameraName;" << strUserName;
		printf("Find Camera:%s\n", strUserName);
	}
}

void CParameterSetting::LoadFirstImage() 
{
	//if (m_bFirstAlgoSuccess)
	if (1)
	{
		QString imagePath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("图像文件"), "", "*.bmp;*.jpg;*.png;;All Files(*)");
		if (imagePath.isEmpty())
		{
			return;
		}
		qDebug() << "load first image:" << imagePath;
		QByteArray ba = imagePath.toLocal8Bit();
		char *file = ba.data();
		m_FirstOriginalImage = imread(file);
		vector<int>Threshold;
		m_FirstRenderImage.create(m_FirstOriginalImage.rows, m_FirstOriginalImage.cols, m_FirstOriginalImage.type());
		QImage qImage;
		if (ui.checkBox_RenderFirst->isChecked())
		{
			qImage = MattoQImage(m_FirstRenderImage);
		}
		else
		{
			qImage = MattoQImage(m_FirstOriginalImage);
		}
		QString style;
		//if (AlgoRunData.bOK)
		//{
		//	style = "image: url(:/CMainWindow/Resources/OK.svg);";
		//}
		//else
		//{
		//	style = "image: url(:/CMainWindow/Resources/NG.svg);";
		//}
		ui.label_FirstTime->setText(QString::fromLocal8Bit("耗时:") + QString::number(1) + "ms");
		ui.label_FirstResult->setStyleSheet(style);
		ui.label_FirstImage->SetImage(qImage);
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("工位1算法模型未初始化成功"));
	}
}

void CParameterSetting::SwitchFirstCameraStatus(int index, bool checked)
{
	qDebug() << "SwitchFirstCameraStatus:" << index;
	printf("SwitchFirstCameraStatus:%d\n", index);
	if (checked)
	{
		ui.pushButton_TriggerFirst->setEnabled(false);
		switch (index)
		{
		case 1:
			m_FirstCameraInfo.CameraHandle.SetEnumValue("TriggerMode", MV_TRIGGER_MODE_OFF);
			break;
		case 2:
			m_FirstCameraInfo.CameraHandle.SetEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
			m_FirstCameraInfo.CameraHandle.SetEnumValue("TriggerSource", MV_TRIGGER_SOURCE_LINE0);
			break;
		case 3:
			m_FirstCameraInfo.CameraHandle.SetEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
			m_FirstCameraInfo.CameraHandle.SetEnumValue("TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);
			ui.pushButton_TriggerFirst->setEnabled(true);
			break;
		}
	}
}

void CParameterSetting::SetButtonGroupEnabled(bool enabled, int index)
{
	if (index == 1)
	{
		ui.radioButton_FreeFirst->setEnabled(enabled);
		ui.radioButton_ExternalFirst->setEnabled(enabled);
		ui.radioButton_SoftFirst->setEnabled(enabled);
	}
	else if (index == 2)
	{

	}
}

bool CParameterSetting::OpenCamera(MV_CC_DEVICE_INFO device_info, int index)
{
	qDebug() << "OpenCamera:" << index;
	printf("open camere:%d\n", index);
	if (index == 1)
	{
		int nRet = m_FirstCameraInfo.CameraHandle.Open(&device_info);
		if (nRet != MV_OK)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("打开一工位相机失败:") + QString::number(nRet));
			return false;
		}
		else
		{
			unsigned int PackSize = 0;
			int nRet = m_FirstCameraInfo.CameraHandle.GetOptimalPacketSize(&PackSize);
			if (nRet == MV_OK)
			{
				m_FirstCameraInfo.CameraHandle.SetIntValue("GevSCPSPacketSize", PackSize);
                GetGain(m_FirstCameraInfo.CameraHandle);
                GetExposureTime(m_FirstCameraInfo.CameraHandle);
			}
			else
			{
				QMessageBox::information(this, QString::fromLocal8Bit("错误"), "First Camera GevSCPSPacketSize Error:" + QString::number(nRet));
				return false;
			}
			nRet = m_FirstCameraInfo.CameraHandle.StartGrabbing();
			if (nRet != MV_OK)
			{
				QMessageBox::information(this, QString::fromLocal8Bit("错误"), "First Camera Start grabbing Error:" + QString::number(nRet));
				return false;
			}
			m_FirstCameraInfo.CameraHandle.SetEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
			m_FirstCameraInfo.CameraHandle.SetEnumValue("TriggerSource", MV_TRIGGER_SOURCE_LINE0);
		}
	}
	else if (index == 2)
	{
		//int nRet = m_SecondCameraInfo.CameraHandle.Open(&device_info);
		//if (nRet != MV_OK)
		//{
		//	QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("打开二工位相机失败:") + QString::number(nRet));
		//	return false;
		//}
		//else
		//{
		//	unsigned int PackSize = 0;
		//	int nRet = m_SecondCameraInfo.CameraHandle.GetOptimalPacketSize(&PackSize);
		//	if (nRet == MV_OK)
		//	{
		//		m_SecondCameraInfo.CameraHandle.SetIntValue("GevSCPSPacketSize", PackSize);
		//	}
		//	else
		//	{
		//		QMessageBox::information(this, QString::fromLocal8Bit("错误"), "Second Camera GevSCPSPacketSize Error:" + QString::number(nRet));
		//		return false;
		//	}
		//	nRet = m_SecondCameraInfo.CameraHandle.StartGrabbing();
		//	if (nRet != MV_OK)
		//	{
		//		QMessageBox::information(this, QString::fromLocal8Bit("错误"), "Second Camera Start grabbing Error:" + QString::number(nRet));
		//		return false;
		//	}
		//	m_SecondCameraInfo.CameraHandle.SetEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
		//	m_SecondCameraInfo.CameraHandle.SetEnumValue("TriggerSource", MV_TRIGGER_SOURCE_LINE0);
		//}
	}
	return true;
}

QImage CParameterSetting::MattoQImage(Mat image)
{
	switch (image.type())
	{
	case CV_8UC1:
	{
		QImage img((const unsigned char *)(image.data), image.cols, image.rows, image.cols, QImage::Format_Grayscale8);
		return img;
	}
	break;
	case CV_8UC3:
	{
		QImage img((const unsigned char *)(image.data), image.cols, image.rows, image.cols * 3, QImage::Format_RGB888);
		return img.rgbSwapped();
	}
	break;
	default:
	{
		QImage img;
		return img;
	}
	break;
	}
}

void CParameterSetting::StartDetecion(bool bStart)
{
	if (bStart)
	{
		m_FirstCameraInfo.ImageCapture->SetSystemType(CAMERA_FIRST);
		//m_SecondCameraInfo.ImageCapture->SetSystemType(CAMERA_FIRST);
	}
	else
	{
		//SetSystemType(ui.comboBox_SystemType->currentIndex());
	}
	m_FirstCameraInfo.ImageCapture->SetRunStatus(bStart);
	//m_SecondCameraInfo.ImageCapture->SetRunStatus(bStart);
	//CSerialManager::GetInstance()->OpenHeart(bStart);
}

void CParameterSetting::OpenFirstCamera()
{
	qDebug() << "open first camera";
	printf("open first camera\n");
	QString name = ui.comboBox_First->currentText();
	int index = ui.comboBox_First->currentIndex();
	if (m_FirstCameraInfo.bOpenCamera)
	{
		bool rv = CloseDevice(1);
		if (!rv)
		{
			return;
		}
		ui.pushButton_OpenFirst->setText(QString::fromLocal8Bit("打开相机"));
		SetButtonGroupEnabled(false, 1);
		//ui.pushButton_SaveConfig->setEnabled(false);
		ui.pushButton_TriggerFirst->setEnabled(false);
		m_FirstCameraInfo.bOpenCamera = false;
		m_FirstCameraInfo.CameraName.clear();
	}
	else
	{
		if (name == m_SecondCameraInfo.CameraName)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("相机已被占用"));
			return;
		}
		else if (index <= m_stDevList.nDeviceNum)
		{
			QString errMsg;
			m_FirstCameraInfo.bOpenCamera = OpenCamera(*m_stDevList.pDeviceInfo[index], 1);
			if (!m_FirstCameraInfo.bOpenCamera)
			{
				return;
			}
			m_FirstCameraInfo.ImageCapture->SetCameraHandle(m_FirstCameraInfo.CameraHandle, 1);
			m_FirstCameraInfo.ImageCapture->SetCameraStatus(true);
			m_FirstCameraInfo.CameraName = name;
			SetButtonGroupEnabled(true, 1);
			//ui.pushButton_SaveConfig->setEnabled(m_SecondCameraInfo.bOpenCamera);
			ui.radioButton_FreeFirst->setChecked(false);
			ui.radioButton_ExternalFirst->setChecked(true);
			ui.radioButton_SoftFirst->setChecked(false);
			ui.pushButton_TriggerFirst->setEnabled(false);
			ui.pushButton_OpenFirst->setText(QString::fromLocal8Bit("关闭相机"));
		}
	}
}

void CParameterSetting::ReceivaAlgoImage( Mat OriginalImage, e_CameraType type, int Time, bool bok)
{
	emit SendAlgoImageToMainWindow(OriginalImage, type, Time, bok);
}

void CParameterSetting::SaveCameraParams1()
{
    qDebug() << "参数设置buttonParamsSet";
    update();
    bool bIsSetSucceed = true;
    int nRet = SetExposureTime(m_FirstCameraInfo.CameraHandle);
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        qDebug() << "Set Exposure Time Fail";
        ShowErrorMsg(("Set Exposure Time Fail"), nRet);
    }
    nRet = SetGain(m_FirstCameraInfo.CameraHandle);
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        qDebug() << "Set Gain Fail";
        ShowErrorMsg(("Set Gain Fail"), nRet);
    }

    if (true == bIsSetSucceed)
    {
        qDebug() << "Set Parameter Succeed";
        ShowErrorMsg(("Set Parameter Succeed"), nRet);
    }

    return;
}

// ch:设置曝光时间 | en:Set Exposure Time
int CParameterSetting::SetExposureTime(const CMvCamera &CameraHandle)
{
    // ch:调节这两个曝光模式，才能让曝光时间生效
    // en:Adjust these two exposure mode to allow exposure time effective
    m_FirstCameraInfo.CameraHandle = CameraHandle;
    int nRet = m_FirstCameraInfo.CameraHandle.SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    nRet = m_FirstCameraInfo.CameraHandle.SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
    m_dExposureEdit = ui.le_exposure_1->text().toFloat();
    nRet = m_FirstCameraInfo.CameraHandle.SetFloatValue("ExposureTime", (float)m_dExposureEdit);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    return MV_OK;
}
// ch:获取曝光时间 | en:Get Exposure Time
int CParameterSetting::GetExposureTime(const CMvCamera &CameraHandle)
{
    MVCC_FLOATVALUE stFloatValue = {0};
    m_FirstCameraInfo.CameraHandle = CameraHandle;
    int nRet = m_FirstCameraInfo.CameraHandle.GetFloatValue("ExposureTime", &stFloatValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }
    // ui.lineEdit_exposure->setText(QString::number(stFloatValue.fCurValue));

    if (ui.le_exposure_1->text().isEmpty())
    {
        m_dExposureEdit = stFloatValue.fCurValue;
        ui.le_exposure_1->setText(QString::number(m_dExposureEdit));
    }
    else
    {
        m_dExposureEdit = ui.le_exposure_1->text().toFloat();
    }

    return MV_OK;
}

// ch:获取增益 | en:Get Gain
int CParameterSetting::GetGain(const CMvCamera &CameraHandle)
{
    MVCC_FLOATVALUE stFloatValue = {0};
    m_FirstCameraInfo.CameraHandle = CameraHandle;
    int nRet = m_FirstCameraInfo.CameraHandle.GetFloatValue("Gain", &stFloatValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    if (ui.le_gain_1->text().isEmpty())
    {
        m_dGainEdit = (int)stFloatValue.fCurValue;
        ui.le_gain_1->setText(QString::number(m_dGainEdit));
    }
    else
    {
        m_dGainEdit = ui.le_gain_1->text().toInt();
    }

    return MV_OK;
}

// ch:设置增益 | en:Set Gain
int CParameterSetting::SetGain(const CMvCamera &CameraHandle)
{
    // ch:设置增益前先把自动增益关闭，失败无需返回
    // en:Set Gain after Auto Gain is turned off, this failure does not need to return
    m_FirstCameraInfo.CameraHandle = CameraHandle;
    int nRet = m_FirstCameraInfo.CameraHandle.SetEnumValue("GainAuto", 0);
    m_dGainEdit = ui.le_gain_1->text().toInt();
    return m_FirstCameraInfo.CameraHandle.SetFloatValue("Gain", (float)m_dGainEdit);
}

// ch:显示错误信息 | en:Show error message
void CParameterSetting::ShowErrorMsg(QString Message, int nErrorNum)
{
    QString errorMsg;
    if (nErrorNum == 0)
    {
        errorMsg = Message;
    }
    else
    {
        errorMsg = QString("%1: error = %2: ").arg(Message).arg(nErrorNum);
    }

    switch (nErrorNum)
    {
        case MV_E_HANDLE:
            errorMsg += "Error or invalid handle ";
            break;
        case MV_E_SUPPORT:
            errorMsg += "Not supported function ";
            break;
        case MV_E_BUFOVER:
            errorMsg += "Cache is full ";
            break;
        case MV_E_CALLORDER:
            errorMsg += "Function calling order error ";
            break;
        case MV_E_PARAMETER:
            errorMsg += "Incorrect parameter ";
            break;
        case MV_E_RESOURCE:
            errorMsg += "Applying resource failed ";
            break;
        case MV_E_NODATA:
            errorMsg += "No data ";
            break;
        case MV_E_PRECONDITION:
            errorMsg += "Precondition error, or running environment changed ";
            break;
        case MV_E_VERSION:
            errorMsg += "Version mismatches ";
            break;
        case MV_E_NOENOUGH_BUF:
            errorMsg += "Insufficient memory ";
            break;
        case MV_E_ABNORMAL_IMAGE:
            errorMsg += "Abnormal image, maybe incomplete image because of lost packet ";
            break;
        case MV_E_UNKNOW:
            errorMsg += "Unknown error ";
            break;
        case MV_E_GC_GENERIC:
            errorMsg += "General error ";
            break;
        case MV_E_GC_ACCESS:
            errorMsg += "Node accessing condition error ";
            break;
        case MV_E_ACCESS_DENIED:
            errorMsg += "No permission ";
            break;
        case MV_E_BUSY:
            errorMsg += "Device is busy, or network disconnected ";
            break;
        case MV_E_NETER:
            errorMsg += "Network error ";
            break;
    }
    QMessageBox::information(NULL, "PROMPT", errorMsg, QMessageBox::Yes || QMessageBox::No, QMessageBox::Yes);
}

void CParameterSetting::getCameraParams()
{
    qDebug() << "获取参数buttonParamsGet";

    //int nRet = GetTriggerMode();
    //if (nRet != MV_OK)
    //{
    //    qDebug() << "Get Trigger Mode Fail";
    //    ShowErrorMsg(("Get Trigger Mode Fail"), nRet);
    //}

    int nRet = GetExposureTime(m_FirstCameraInfo.CameraHandle);
    if (nRet != MV_OK)
    {
        qDebug() << "Get Exposure Time Fail";
        ShowErrorMsg(("Get Exposure Time Fail"), nRet);
    }

    nRet = GetGain(m_FirstCameraInfo.CameraHandle);
    if (nRet != MV_OK)
    {
        qDebug() << "Get Gain Fail";
        ShowErrorMsg(("Get Gain Fail"), nRet);
    }

    //nRet = GetTriggerSource();
    //if (nRet != MV_OK)
    //{
    //    qDebug() << "Get Trigger Source Fail";
    //    ShowErrorMsg(("Get Trigger Source Fail"), nRet);
    //}
    update();

    return;
}