using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using DPSDK_RES = System.Int32;

namespace DPSDK_CSharpDemo
{
    public partial class CSharpDemo : Form
    {
        private void trackBarPreviewVolume_ValueChanged(object sender, EventArgs e)
        {
            labelPreviewVolume.Text = trackBarPreviewVolume.Value.ToString();
        }

        private void radioPreviewSinglePlay_CheckedChanged(object sender, EventArgs e)
        {
            if (radioPreviewSinglePlay.Checked)
            {
                textPreviewDeviceID.Enabled = true;
                textPreviewStreamType.Enabled = true;
                textPreviewDataType.Enabled = true;
                textPreviewScreenNum.Enabled = false;
                textPreviewTrackID.Enabled = false;
                textPreviewStartIndex.Enabled = false;

                btnPreviewStartVideo.Enabled = true;
                btnPreviewStopVideo.Enabled = true;
            }
        }

        private void radioPreviewMultiviewPlay_CheckedChanged(object sender, EventArgs e)
        {
            if (radioPreviewMultiviewPlay.Checked)
            {
                textPreviewDeviceID.Enabled = false;
                textPreviewStreamType.Enabled = false;
                textPreviewDataType.Enabled = false;
                textPreviewScreenNum.Enabled = true;
                textPreviewTrackID.Enabled = true;
                textPreviewStartIndex.Enabled = true;

                btnPreviewStartVideo.Enabled = false;
                btnPreviewStopVideo.Enabled = false;
            }
        }

        private void btnPreviewStartVideo_Click(object sender, EventArgs e)
        {
            if (radioPreviewSinglePlay.Checked)
            {
                DPSDK_REALPLAY_PARAM pRealPlayParam = new DPSDK_REALPLAY_PARAM();
                pRealPlayParam.struMediaBaseParam.pHWnd = pictureBoxPreviewVideo.Handle;
                pRealPlayParam.struMediaBaseParam.szCodeId = textPreviewDeviceID.Text;
                pRealPlayParam.struMediaBaseParam.iStreamType = Convert.ToInt32(textPreviewStreamType.Text);
                pRealPlayParam.struMediaBaseParam.iDataType = Convert.ToInt32(textPreviewDataType.Text);
                pRealPlayParam.struMediaBaseParam.iDecodeType = (int)(checkBoxPreviewDecode.CheckState == CheckState.Checked ? DPSDK_DECODE_TYPE.DPSDK_DECODE_HW : DPSDK_DECODE_TYPE.DPSDK_DECODE_SW);
                pRealPlayParam.struMediaBaseParam.iStreamMode = Convert.ToInt32(textPreviewStreamMode.Text);
                pRealPlayParam.struMediaBaseParam.uiDelayTime = Convert.ToUInt32(textPreviewDelayTime.Text);
                DPSDK_RES iRet = DPSDK_StartRealPlay(m_iSessionID, ref pRealPlayParam, ref m_iPreviewMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    textPreviewResult.Text = AppData._CS("start video success");
                }
                else
                {
                    textPreviewResult.Text = AppData._CS("start video failed, iRet = %1", iRet.ToString());
                }
            }
            else
            {

            }
        }

        private void btnPreviewStopVideo_Click(object sender, EventArgs e)
        {
            if (m_iPreviewMediaSessionID != IntPtr.Zero)
            {
                DPSDK_RES iRet = DPSDK_StopRealPlay(m_iSessionID, m_iPreviewMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    textPreviewResult.Text = AppData._CS("stop video success");
                    pictureBoxPreviewVideo.Image = null;
                }
                else
                {
                    textPreviewResult.Text = AppData._CS("stop video failed, iRet = %1", iRet.ToString());
                }
                m_iPreviewMediaSessionID = IntPtr.Zero;
            }
        }

        private void btnPreviewGetStreamMode_Click(object sender, EventArgs e)
        {
            IntPtr pStreamMode = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_GetPlayStreamMode(m_iSessionID, m_iPreviewMediaSessionID, ref pStreamMode);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewStreamMode.Text = pStreamMode.ToString();
                textPreviewResult.Text = AppData._CS("Get Stream Mode success. uiStreamMode = %1", pStreamMode.ToString());
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Get Stream Mode failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewSetStreamMode_Click(object sender, EventArgs e)
        {
            if (textPreviewStreamMode.Text == "" || textPreviewDelayTime.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }

            uint uiStreamMode = Convert.ToUInt32(textPreviewStreamMode.Text);
            uint uiDelayTime = Convert.ToUInt32(textPreviewDelayTime.Text);
            DPSDK_RES iRet = DPSDK_SetPlayStreamMode(m_iSessionID, m_iPreviewMediaSessionID, uiStreamMode, uiDelayTime);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Set Stream Mode success.");
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Set Stream Mode failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewSetRegion_Click(object sender, EventArgs e)
        {
            if (textPreviewTop.Text == "" || textPreviewBottom.Text == "" || textPreviewLeft.Text == "" || textPreviewRight.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }

            DPSDK_RECT pRECT = new DPSDK_RECT();
            pRECT.top = Convert.ToInt64(textPreviewTop.Text);
            pRECT.bottom = Convert.ToInt64(textPreviewBottom.Text);
            pRECT.left = Convert.ToInt64(textPreviewLeft.Text);
            pRECT.right = Convert.ToInt64(textPreviewRight.Text);
            DPSDK_RES iRet = DPSDK_SetDisplayRegion(m_iSessionID, m_iPreviewMediaSessionID, ref pRECT, pictureBoxPreviewVideo.Handle, checkBoxPreviewEnable.Checked);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Set Display Region success.");
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Set Display Region failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewLockVideo_Click(object sender, EventArgs e)
        {
            if (btnPreviewLockVideo.Text == AppData._CS("LockVideo"))
            {
                DPSDK_RES iRet = DPSDK_OperateRealPlay(m_iSessionID, m_iPreviewMediaSessionID, textPreviewDeviceID.Text, (int)DPSDK_VIDEO_LOCK_TYPE.DPSDK_VIDEO_CMD_LOCK);
                if (iRet == DPSDK_SUCCESS)
                {
                    btnPreviewLockVideo.Text = AppData._CS("UnlockVideo");
                    textPreviewResult.Text = AppData._CS("Lock video success.");
                }
                else
                {
                    textPreviewResult.Text = AppData._CS("Lock video failed, error code = %1", iRet.ToString());
                }
            }
            else if (btnPreviewLockVideo.Text == AppData._CS("UnlockVideo"))
            {
                DPSDK_RES iRet = DPSDK_OperateRealPlay(m_iSessionID, m_iPreviewMediaSessionID, textPreviewDeviceID.Text, (int)DPSDK_VIDEO_LOCK_TYPE.DPSDK_VIDEO_CMD_UNLOCK_ONE);
                if (iRet == DPSDK_SUCCESS)
                {
                    btnPreviewLockVideo.Text = AppData._CS("LockVideo");
                    textPreviewResult.Text = AppData._CS("Unlock video success.");
                }
                else
                {
                    textPreviewResult.Text = AppData._CS("Unlock video failed, error code = %1", iRet.ToString());
                }
            }
        }

        private void btnPreviewGetPicSize_Click(object sender, EventArgs e)
        {
            IntPtr pWidth = IntPtr.Zero;
            IntPtr pHeight = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_GetPictureSize(m_iSessionID, m_iPreviewMediaSessionID, ref pWidth, ref pHeight);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Get Picture Size success. lWidth = %1, lHeight = %2", pWidth.ToInt32().ToString(), pHeight.ToInt32().ToString());
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Get Picture Size failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewGetPicToFile_Click(object sender, EventArgs e)
        {
            uint uiPicFormat = (uint)comboBoxPreviewPicture.SelectedIndex;
            string strPicturePath = textPreviewPicPath.Text;
            DPSDK_RES iRet = DPSDK_Get24BitPictureFile(m_iSessionID, m_iPreviewMediaSessionID, uiPicFormat, strPicturePath);
            if (iRet == DPSDK_SUCCESS)
            {
                pictureBoxPreviewPicture.Image = Image.FromFile(strPicturePath);
                textPreviewResult.Text = AppData._CS("Get Picture To File success.");
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Get Picture To File failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewGetPicToBuffer_Click(object sender, EventArgs e)
        {
            IntPtr pWidth = IntPtr.Zero;
            IntPtr pHeight = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_GetPictureSize(m_iSessionID, m_iPreviewMediaSessionID, ref pWidth, ref pHeight);
            if (iRet != DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Get Picture Size failed, iRet = %1", iRet.ToString());
                return;
            }

            int iBufsize = GetPicBuffSize(comboBoxPreviewPicture.SelectedIndex, pHeight.ToInt32(), pWidth.ToInt32());
            byte[] pPicBuf = new byte[iBufsize];
            IntPtr pPicSize = IntPtr.Zero;
            iRet = DPSDK_GetPictureBuf(m_iSessionID, m_iPreviewMediaSessionID, ref pPicBuf[0], (IntPtr)iBufsize, ref pPicSize, (uint)comboBoxPreviewPicture.SelectedIndex);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Get Picture To Buffer success. lBufSize = %1, iFactPicSize = %2", iBufsize.ToString() , pPicSize.ToString());
                MemoryStream ms = new MemoryStream(pPicBuf);
                pictureBoxPreviewPicture.Image = Image.FromStream(ms);
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Get Picture To File Buffer, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewOpenSound_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_OpenSound(m_iSessionID, m_iPreviewMediaSessionID);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Open Sound success.");
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Open Sound failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewCloseSound_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_CloseSound(m_iSessionID, m_iPreviewMediaSessionID);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Close Sound success.");
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Close Sound failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewOpenShareSound_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_OpenSoundShare(m_iSessionID, m_iPreviewMediaSessionID);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Open Share Sound success.");
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Open Share Sound failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewCloseShareSound_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_CloseSoundShare(m_iSessionID, m_iPreviewMediaSessionID);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Close Share Sound success.");
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Close Share Sound failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewIsOpenSound_Click(object sender, EventArgs e)
        {
            IntPtr pIsOpenSound = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_IsOpenSoundState(m_iSessionID, m_iPreviewMediaSessionID, ref pIsOpenSound);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Is Open Sound State success. bIsOpen = %1", pIsOpenSound.ToString());
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Is Open Sound State failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewSetVolume_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_SetVolume(m_iSessionID, m_iPreviewMediaSessionID, (uint)trackBarPreviewVolume.Value);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Set Volume success.");
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Set Volume failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewGetVolum_Click(object sender, EventArgs e)
        {
            IntPtr pVolume = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_GetVolume(m_iSessionID, m_iPreviewMediaSessionID, ref pVolume);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Get Volume success. iVolume = %1", pVolume.ToString());
                labelPreviewVolume.Text = pVolume.ToString();
                trackBarPreviewVolume.Value = pVolume.ToInt32();
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Get Volume failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewSetSplitLen_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_SetSplitRecordLen(m_iSessionID, m_iPreviewMediaSessionID, (uint)Convert.ToUInt32(textPreviewSplitLen.Text));
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Set Split Record Len success.");
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Set Split Record Len failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewStartRecord_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_StartRecord(m_iSessionID, m_iPreviewMediaSessionID, textPreviewRecordPath.Text, (uint)Convert.ToUInt32(textPreviewSplitLen.Text));
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Start Record success.");
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Start Record failed, iRet = %1", iRet.ToString());
            }
        }

        private void btnPreviewStopRecord_Click(object sender, EventArgs e)
        {
            uint uiBufLen = (uint)(Marshal.SizeOf(typeof(UInt64)) + Marshal.SizeOf(typeof(DPSDK_FILE_STORE_INFO)) * 100);
            IntPtr pRecordFile = Marshal.AllocHGlobal((int)uiBufLen);
            DPSDK_RES iRet = DPSDK_StopRecord(m_iSessionID, m_iPreviewMediaSessionID, pRecordFile, uiBufLen);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Stop Record success.") + "\r\n";
                DPSDK_FILE_STORE_LIST stuRecordFile = new DPSDK_FILE_STORE_LIST();
                stuRecordFile = (DPSDK_FILE_STORE_LIST)Marshal.PtrToStructure(pRecordFile, typeof(DPSDK_FILE_STORE_LIST));
                if (stuRecordFile.uiTotal > 0)
                {
                    DPSDK_FILE_STORE_INFO[] pFileList = new DPSDK_FILE_STORE_INFO[stuRecordFile.uiTotal];
                    for (int i = 0; i < (int)stuRecordFile.uiTotal; i++)
                    {
                        IntPtr ptr = new IntPtr(pRecordFile.ToInt32() + Marshal.SizeOf(typeof(UInt64)) + Marshal.SizeOf(typeof(DPSDK_FILE_STORE_INFO)) * i);
                        pFileList[i] = (DPSDK_FILE_STORE_INFO)Marshal.PtrToStructure(ptr, typeof(DPSDK_FILE_STORE_INFO));
                        textPreviewResult.Text += "uiStoreLen = " + pFileList[i].uiStoreLen.ToString() + ", lBeginTime = " + pFileList[i].lBeginTime.ToString() + ", lEndTimeTime = " + pFileList[i].lEndTime.ToString() + ", szFile = " + pFileList[i].szFile + "\r\n";
                    }
                }
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Stop Record failed, iRet = %1", iRet.ToString());
            }
            Marshal.FreeHGlobal(pRecordFile);
            
        }

        private void btnPreviewIsRecordState_Click(object sender, EventArgs e)
        {
            IntPtr pIsRecord = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_IsRecordState(m_iSessionID, m_iPreviewMediaSessionID, ref pIsRecord);
            if (iRet == DPSDK_SUCCESS)
            {
                textPreviewResult.Text = AppData._CS("Get Is Record State success. bIsRecord = %1", pIsRecord.ToString());
            }
            else
            {
                textPreviewResult.Text = AppData._CS("Get Is Record State failed, iRet = %1", iRet.ToString());
            }
        }
    }
}