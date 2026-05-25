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
        private void trackBarPlaybackVolume_ValueChanged(object sender, EventArgs e)
        {
            labelPlaybackVolume.Text = trackBarPlaybackVolume.Value.ToString();
        }

        void fEventDownloadCallback(IntPtr iEventType, IntPtr iMediaSessionID, IntPtr pData, IntPtr pUserParam)
        {
            switch (iEventType.ToInt32())
            {
                case DPSDK_EVENT_DOWNLOAD_FILE_STARTPLAYBACK:
                    {
                        //Start download by file
                        if (pData != IntPtr.Zero)
                        {
                            textPlaybackResult.Text = "Start Download Record By File.";
                        }
                    }
                    break;
                case DPSDK_EVENT_DOWNLOAD_TIME_STARTPLAYBACK:
                    {
                        //Start download by time
                        if (pData != IntPtr.Zero)
                        {
                            textPlaybackResult.Text = "Start Download Record By Time.";
                        }
                    }
                    break;
                case DPSDK_EVENT_DOWNLOAD_PROGRESS:
                    {
                        //Download progress
                        if (pData != IntPtr.Zero)
                        {
                            textPlaybackResult.Text = "Download Progress...";
                        }
                    }
                    break;
                case DPSDK_EVENT_DOWNLOAD_CUT_FILE:
                    {
                        //Cut file finish
                        if (pData != IntPtr.Zero)
                        {
                            textPlaybackResult.Text = "Download Finish!";
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        private void btnPlaybackQueryRecord_Click(object sender, EventArgs e)
        {
            listViewPlaybackRecordInfo.Items.Clear();

            DPSDK_QUERY_RECORD_PARAM pQueryRecord = new DPSDK_QUERY_RECORD_PARAM();
            pQueryRecord.szCameraId = textPlaybackDeviceID.Text;
            pQueryRecord.iRecordType = (DPSDK_RECORD_TYPE)Convert.ToInt32(comboBoxPlaybackQueryRecordType.Text.Substring(0, comboBoxPlaybackQueryRecordType.Text.IndexOf("-")));
            pQueryRecord.iSourceType = (DPSDK_SOURCE_TYPE)Convert.ToInt32(comboBoxPlaybackSourceType.Text.Substring(0, comboBoxPlaybackSourceType.Text.IndexOf("-")));
            pQueryRecord.iStreamType = (DPSDK_STREAM_TYPE)Convert.ToInt32(comboBoxPlaybackStreamType.Text.Substring(0, comboBoxPlaybackStreamType.Text.IndexOf("-")));
            pQueryRecord.tBeginTime = Convert.ToInt64(datePlaybackStartTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);
            pQueryRecord.tEndTime = Convert.ToInt64(datePlaybackEndTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);

            uint uiBufLen = (uint)(64 + Marshal.SizeOf(typeof(/*UInt32*/Int64)) + Marshal.SizeOf(typeof(DPSDK_SINGLE_RECORD_INFO)) * 100);//Since C++ struct byte alignment£¬uint type iRetCount is really 8 byte size£¬so use Int64 instand
            IntPtr pRecordList = Marshal.AllocHGlobal((int)uiBufLen);
            DPSDK_RES iRet = DPSDK_QueryRecord(m_iSessionID, ref pQueryRecord, pRecordList, uiBufLen);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "QueryRecord success";
                DPSDK_RECORD_INFO_LIST stuRecordList = new DPSDK_RECORD_INFO_LIST();
                stuRecordList = (DPSDK_RECORD_INFO_LIST)Marshal.PtrToStructure(pRecordList, typeof(DPSDK_RECORD_INFO_LIST));
                if (stuRecordList.iRetCount > 0)
                {
                    for (int i = 0; i < (int)stuRecordList.iRetCount; i++)
                    {
                        DPSDK_SINGLE_RECORD_INFO pSingleRecord = new DPSDK_SINGLE_RECORD_INFO();
                        IntPtr ptr = new IntPtr(pRecordList.ToInt32() + 64 + Marshal.SizeOf(typeof(/*UInt32*/Int64)) + Marshal.SizeOf(typeof(DPSDK_SINGLE_RECORD_INFO)) * i);
                        pSingleRecord = (DPSDK_SINGLE_RECORD_INFO)Marshal.PtrToStructure(ptr, typeof(DPSDK_SINGLE_RECORD_INFO));
                        ListViewItem item = listViewPlaybackRecordInfo.Items.Add(Convert.ToString(i+1));
                        item.SubItems.Add(pSingleRecord.iSourceType.ToString());
                        item.SubItems.Add(pSingleRecord.iRecordType.ToString());
                        item.SubItems.Add(pSingleRecord.iStartTime.ToString());
                        item.SubItems.Add(pSingleRecord.iEndTime.ToString());
                        item.SubItems.Add(pSingleRecord.szName);
                        item.SubItems.Add(pSingleRecord.iLength.ToString());
                        item.SubItems.Add(pSingleRecord.iStreamType.ToString());
                        item.SubItems.Add(pSingleRecord.bLocked.ToString());
                        item.SubItems.Add(pSingleRecord.iSSId.ToString());
                        item.SubItems.Add(pSingleRecord.iFileHandle.ToString());
                        item.SubItems.Add(pSingleRecord.szDiskId);
                    }
                }
            }
            else
            {
                textPlaybackResult.Text = "QueryRecord failed, error code = " + iRet.ToString();
            }
            Marshal.FreeHGlobal(pRecordList);
        }

        private void btnPlaybackQueryRecordDate_Click(object sender, EventArgs e)
        {
            DPSDK_QUERY_RECORD_DATE_PARAM pQueryDateInfo = new DPSDK_QUERY_RECORD_DATE_PARAM();
            pQueryDateInfo.szCameraId = textPlaybackDeviceID.Text;
            pQueryDateInfo.iSourceType = (DPSDK_SOURCE_TYPE)Convert.ToInt32(comboBoxPlaybackSourceType.Text.Substring(0, comboBoxPlaybackSourceType.Text.IndexOf("-")));
            pQueryDateInfo.iYear = datePlaybackStartTime.Value.Year;
            pQueryDateInfo.iMonth = datePlaybackStartTime.Value.Month;
            DPSDK_RECORD_DATE_INFO pRecordDate = new DPSDK_RECORD_DATE_INFO();
            DPSDK_RES iRet = DPSDK_QueryRecordDate(m_iSessionID, ref pQueryDateInfo, ref pRecordDate);
            if (iRet == DPSDK_SUCCESS)
            {
                string strRecordDays = "";
                for (int i = 0; i < 31; i++)
                {
                    strRecordDays += pRecordDate.RecordDays[i].ToString();
                }
                textPlaybackResult.Text = "QueryRecordDate success, RecordDays = " + strRecordDays;
            }
            else
            {
                textPlaybackResult.Text = "QueryRecordDate failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackGetRecordStatus_Click(object sender, EventArgs e)
        {
            DPSDK_RECORD_STATUS_INFO pRecordInfo = new DPSDK_RECORD_STATUS_INFO();
            DPSDK_RES iRet = DPSDK_GetRecordStatus(m_iSessionID, textPlaybackDeviceID.Text, ref pRecordInfo);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "GetRecordStatus success, iRecordStatus = " + pRecordInfo.iRecordStatus.ToString();
            }
            else
            {
                textPlaybackResult.Text = "GetRecordStatus failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackLockRecordFile_Click(object sender, EventArgs e)
        {
            DPSDK_LOCK_RECORD_FILE_PARAM pLockFileInfo = new DPSDK_LOCK_RECORD_FILE_PARAM();
            pLockFileInfo.szCameraId = textPlaybackDeviceID.Text;
            if (listViewPlaybackRecordInfo.SelectedIndices.Count > 0)
            {
                pLockFileInfo.szFilename = listViewPlaybackRecordInfo.SelectedItems[0].SubItems[5].Text;
                pLockFileInfo.szFilename = pLockFileInfo.szFilename.Replace("efs://", "");
            }
            else
            {
                AppData.ShowMsgInfo("Please select one record!");
                return;
            }
            DPSDK_LOCK_RECORD_FILE_RESULT pResult = new DPSDK_LOCK_RECORD_FILE_RESULT();
            DPSDK_RES iRet = DPSDK_LockRecordFile(m_iSessionID, ref pLockFileInfo, ref pResult);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "LockRecordFile success, iLockNum = " + pResult.iLockNum.ToString();
            }
            else
            {
                textPlaybackResult.Text = "LockRecordFile failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackUnlockRecordFile_Click(object sender, EventArgs e)
        {
            DPSDK_UNLOCK_RECORD_FILE_PARAM pUnlockFileInfo = new DPSDK_UNLOCK_RECORD_FILE_PARAM();
            pUnlockFileInfo.szCameraId = textPlaybackDeviceID.Text;
            if (listViewPlaybackRecordInfo.SelectedIndices.Count > 0)
            {
                pUnlockFileInfo.szFilename = listViewPlaybackRecordInfo.SelectedItems[0].SubItems[5].Text;
                pUnlockFileInfo.szFilename = pUnlockFileInfo.szFilename.Replace("efs://", "");
            }
            else
            {
                AppData.ShowMsgInfo("Please select one record!");
                return;
            }
            pUnlockFileInfo.bForce = (byte)(checkBoxPlaybackForce.CheckState == CheckState.Checked ? 1 : 0);
            DPSDK_LOCK_RECORD_FILE_RESULT pResult = new DPSDK_LOCK_RECORD_FILE_RESULT();
            DPSDK_RES iRet = DPSDK_UnlockRecordFile(m_iSessionID, ref pUnlockFileInfo, ref pResult);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "UnlockRecordFile success, iLockNum = " + pResult.iLockNum.ToString();
            }
            else
            {
                textPlaybackResult.Text = "UnlockRecordFile failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackQueryLockRecord_Click(object sender, EventArgs e)
        {
            DPSDK_QUERY_LOCK_RECORD_PARAM pQueryLockRecord = new DPSDK_QUERY_LOCK_RECORD_PARAM();
            pQueryLockRecord.szUserId = textPlaybackUserID.Text;
            pQueryLockRecord.szCameraId = textPlaybackDeviceID.Text;
            pQueryLockRecord.iSourceType = (DPSDK_SOURCE_TYPE)Convert.ToInt32(comboBoxPlaybackSourceType.Text.Substring(0, comboBoxPlaybackSourceType.Text.IndexOf("-")));
            pQueryLockRecord.tStartTime = Convert.ToInt64(datePlaybackStartTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);
            pQueryLockRecord.tEndTime = Convert.ToInt64(datePlaybackEndTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);

            uint uiBufLen = (uint)(64 + Marshal.SizeOf(typeof(int)) + Marshal.SizeOf(typeof(DPSDK_SINGLE_RECORD_LOCK_INFO)) * 10);
            IntPtr pLockRecordList = Marshal.AllocHGlobal((int)uiBufLen);
            DPSDK_RES iRet = DPSDK_QueryLockRecord(m_iSessionID, ref pQueryLockRecord, pLockRecordList, uiBufLen);
            if (iRet == DPSDK_SUCCESS)
            {
                DPSDK_RECORD_LOCK_INFO_LIST stuLockRecordList = new DPSDK_RECORD_LOCK_INFO_LIST();
                stuLockRecordList = (DPSDK_RECORD_LOCK_INFO_LIST)Marshal.PtrToStructure(pLockRecordList, typeof(DPSDK_RECORD_LOCK_INFO_LIST));
                textPlaybackResult.Text = "QueryLockRecord success, iRetCount = " + stuLockRecordList.iRetCount.ToString();
            }
            else
            {
                textPlaybackResult.Text = "QueryLockRecord failed, error code = " + iRet.ToString();
            }
            Marshal.FreeHGlobal(pLockRecordList);
        }

        private void btnPlaybackStopRemoteRecord_Click(object sender, EventArgs e)
        {
            DPSDK_PTZOPERATE_STOPREMOTERECORD_PARAM pStopRemoteRecordParam = new DPSDK_PTZOPERATE_STOPREMOTERECORD_PARAM();
            pStopRemoteRecordParam.szChannelId = textPlaybackDeviceID.Text;
            pStopRemoteRecordParam.iStreamType = Convert.ToInt32(comboBoxPlaybackStreamType.Text.Substring(0, comboBoxPlaybackStreamType.Text.IndexOf("-")));
            pStopRemoteRecordParam.bForce = (byte)(checkBoxPlaybackForce.CheckState == CheckState.Checked ? 1 : 0);
            DPSDK_PTZOPERATE_REMOTERECORD_RESULT pStopRemoteRecordResult = new DPSDK_PTZOPERATE_REMOTERECORD_RESULT();
            DPSDK_RES iRet = DPSDK_StopRemoteRecord(m_iSessionID, ref pStopRemoteRecordParam, ref pStopRemoteRecordResult);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "StopRemoteRecord success";
            }
            else
            {
                textPlaybackResult.Text = "StopRemoteRecord failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackStartRemoteRecord_Click(object sender, EventArgs e)
        {
            if (textPlaybackDuration.Text == "")
            {
                AppData.ShowMsgInfo("Please input correct params!");
                return;
            }
            DPSDK_PTZOPERATE_STARTREMOTERECORD_PARAM pStartRemoteRecordParam = new DPSDK_PTZOPERATE_STARTREMOTERECORD_PARAM();
            pStartRemoteRecordParam.szChannelId = textPlaybackDeviceID.Text;
            pStartRemoteRecordParam.iStreamType = Convert.ToInt32(comboBoxPlaybackStreamType.Text.Substring(0, comboBoxPlaybackStreamType.Text.IndexOf("-")));
            pStartRemoteRecordParam.iRecordDuration = Convert.ToInt32(textPlaybackDuration.Text);
            DPSDK_PTZOPERATE_REMOTERECORD_RESULT pStartRemoteRecordResult = new DPSDK_PTZOPERATE_REMOTERECORD_RESULT();
            DPSDK_RES iRet = DPSDK_StartRemoteRecord(m_iSessionID, ref pStartRemoteRecordParam, ref pStartRemoteRecordResult);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "StartRemoteRecord success";
            }
            else
            {
                textPlaybackResult.Text = "StartRemoteRecord failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackByTime_Click(object sender, EventArgs e)
        {
            if (m_iPlaybackMediaSessionID.ToInt32() > 0)
            {
                DPSDK_RES iRet = DPSDK_StopPlayback(m_iSessionID, m_iPlaybackMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    textPlaybackResult.Text = "StopPlayback success";
                }
                else
                {
                    textPlaybackResult.Text = "StopPlayback failed, error code = " + iRet.ToString();
                }

                m_iPlaybackMediaSessionID = IntPtr.Zero;
                btnPlaybackByTime.Text = "PlaybackByTime";
                btnPlaybackPausePlayback.Text = "PausePlayback";
            }
            else
            {
                DPSDK_PLAYBACK_BY_TIME_PARAM pPlaybackParam = new DPSDK_PLAYBACK_BY_TIME_PARAM();
                pPlaybackParam.pHWnd = pictureBoxPlaybackVideo.Handle;
                pPlaybackParam.iDirection = Convert.ToInt32(comboBoxPlaybackDirection.Text.Substring(0, comboBoxPlaybackDirection.Text.IndexOf("-")));
                pPlaybackParam.szCodeId = textPlaybackDeviceID.Text;
                pPlaybackParam.tBeginTime = Convert.ToInt64(datePlaybackStartTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);
                pPlaybackParam.tEndTime = Convert.ToInt64(datePlaybackEndTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);
                pPlaybackParam.tPlayTime = pPlaybackParam.tBeginTime;
                pPlaybackParam.iRecordSource = Convert.ToInt32(comboBoxPlaybackSourceType.Text.Substring(0, comboBoxPlaybackSourceType.Text.IndexOf("-")));
                pPlaybackParam.iStreamType = Convert.ToInt32(comboBoxPlaybackStreamType.Text.Substring(0, comboBoxPlaybackStreamType.Text.IndexOf("-")));
                pPlaybackParam.iRecordType = Convert.ToInt32(comboBoxPlaybackQueryRecordType.Text.Substring(0, comboBoxPlaybackQueryRecordType.Text.IndexOf("-")));
                DPSDK_RES iRet = DPSDK_StartPlaybackByTime(m_iSessionID, ref pPlaybackParam, ref m_iPlaybackMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    btnPlaybackByTime.Text = "StopPlayback";
                    textPlaybackResult.Text = "StartPlaybackByTime success";
                }
                else
                {
                    textPlaybackResult.Text = "StartPlaybackByTime failed, error code = " + iRet.ToString();
                }
            }
        }

        private void btnPlaybackFile_Click(object sender, EventArgs e)
        {
            if (m_iPlaybackMediaSessionID.ToInt32() > 0)
            {
                DPSDK_RES iRet = DPSDK_StopPlayback(m_iSessionID, m_iPlaybackMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    textPlaybackResult.Text = "StopPlayback success";
                }
                else
                {
                    textPlaybackResult.Text = "StopPlayback failed, error code = " + iRet.ToString();
                }

                m_iPlaybackMediaSessionID = IntPtr.Zero;
                btnPlaybackFile.Text = "PlaybackByFile";
                btnPlaybackPausePlayback.Text = "PausePlayback";
            }
            else
            {
                DPSDK_PLAYBACK_BY_FILE_PARAM pPlaybackParam = new DPSDK_PLAYBACK_BY_FILE_PARAM();
                pPlaybackParam.pHWnd = pictureBoxPlaybackVideo.Handle;
                pPlaybackParam.iDirection = Convert.ToInt32(comboBoxPlaybackDirection.Text.Substring(0, comboBoxPlaybackDirection.Text.IndexOf("-")));
                pPlaybackParam.szCodeId = textPlaybackDeviceID.Text;
                //pPlaybackParam.tBeginTime = Convert.ToInt64(datePlaybackStartTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);
                //pPlaybackParam.tEndTime = Convert.ToInt64(datePlaybackEndTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);
                pPlaybackParam.iRecordSource = Convert.ToInt32(comboBoxPlaybackSourceType.Text.Substring(0, comboBoxPlaybackSourceType.Text.IndexOf("-")));
                if (listViewPlaybackRecordInfo.SelectedIndices.Count > 0)
                {
                    pPlaybackParam.szFilename = listViewPlaybackRecordInfo.SelectedItems[0].SubItems[5].Text;
                    pPlaybackParam.szFilename = pPlaybackParam.szFilename.Replace("efs://", "");
                    pPlaybackParam.tBeginTime = Convert.ToInt64(listViewPlaybackRecordInfo.SelectedItems[0].SubItems[3].Text);
                    pPlaybackParam.tEndTime = Convert.ToInt64(listViewPlaybackRecordInfo.SelectedItems[0].SubItems[4].Text);
                    pPlaybackParam.uSSId = Convert.ToUInt64(listViewPlaybackRecordInfo.SelectedItems[0].SubItems[9].Text);
                    pPlaybackParam.uFileHandle = Convert.ToUInt64(listViewPlaybackRecordInfo.SelectedItems[0].SubItems[10].Text);
                    pPlaybackParam.szDiskId = listViewPlaybackRecordInfo.SelectedItems[0].SubItems[11].Text;
                }
                else
                {
                    AppData.ShowMsgInfo("Please select one record!");
                    return;
                }
                DPSDK_RES iRet = DPSDK_StartPlaybackByFile(m_iSessionID, ref pPlaybackParam, ref m_iPlaybackMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    btnPlaybackByTime.Text = "StopPlayback";
                    textPlaybackResult.Text = "StartPlaybackByFile success";
                }
                else
                {
                    textPlaybackResult.Text = "StartPlaybackByFile failed, error code = " + iRet.ToString();
                }
            }
        }

        private void btnPlaybackPausePlayback_Click(object sender, EventArgs e)
        {
            if (btnPlaybackPausePlayback.Text == "PausePlayback")
            {
                DPSDK_RES iRet = DPSDK_PlaybackPause(m_iSessionID, m_iPlaybackMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    btnPlaybackPausePlayback.Text = "ResumePlayback";
                    textPlaybackResult.Text = "PlaybackPause success";
                }
                else
                {
                    textPlaybackResult.Text = "PlaybackPause failed, error code = " + iRet.ToString();
                }
            }
            else
            {
                DPSDK_RES iRet = DPSDK_PlaybackResume(m_iSessionID, m_iPlaybackMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    btnPlaybackPausePlayback.Text = "PausePlayback";
                    textPlaybackResult.Text = "PlaybackResume success";
                }
                else
                {
                    textPlaybackResult.Text = "PlaybackResume failed, error code = " + iRet.ToString();
                }
            }
        }

        private void btnPlaybackFrameStep_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_PlaybackFrameStep(m_iSessionID, m_iPlaybackMediaSessionID);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "PlaybackFrameStep success";
            }
            else
            {
                textPlaybackResult.Text = "PlaybackFrameStep failed, error code = " + iRet.ToString();
            }
        }

        private DPSDK_PLAYBACK_SPEED GetSpeed(int iSpeedFormat)
        {
            DPSDK_PLAYBACK_SPEED iSpeed = DPSDK_PLAYBACK_SPEED.DPSDK_PB_NORMAL;
            switch (iSpeedFormat)
            {
                case 0:
                    iSpeed = DPSDK_PLAYBACK_SPEED.DPSDK_PB_NORMAL;
                    break;
                case 1:
                    iSpeed = DPSDK_PLAYBACK_SPEED.DPSDK_PB_NORMAL_FAST2;
                    break;
                case 2:
                    iSpeed = DPSDK_PLAYBACK_SPEED.DPSDK_PB_NORMAL_FAST4;
                    break;
                case 3:
                    iSpeed = DPSDK_PLAYBACK_SPEED.DPSDK_PB_NORMAL_FAST8;
                    break;
                case 4:
                    iSpeed = DPSDK_PLAYBACK_SPEED.DPSDK_PB_NORMAL_FAST16;
                    break;
                case 5:
                    iSpeed = DPSDK_PLAYBACK_SPEED.DPSDK_PB_NORMAL_SLOW2;
                    break;
                case 6:
                    iSpeed = DPSDK_PLAYBACK_SPEED.DPSDK_PB_NORMAL_SLOW4;
                    break;
                case 7:
                    iSpeed = DPSDK_PLAYBACK_SPEED.DPSDK_PB_NORMAL_SLOW8;
                    break;
                case 8:
                    iSpeed = DPSDK_PLAYBACK_SPEED.DPSDK_PB_NORMAL_SLOW16;
                    break;
                default:
                    break;
            }
            return iSpeed;
        }

        private void btnPlaybackSeek_Click(object sender, EventArgs e)
        {
            DPSDK_PLAYBACK_SEEK_PARAM pPlaybackSeekParam = new DPSDK_PLAYBACK_SEEK_PARAM();
            pPlaybackSeekParam.tBeginTime = Convert.ToInt64(datePlaybackStartTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);
            pPlaybackSeekParam.tEndTime = Convert.ToInt64(datePlaybackEndTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);
            pPlaybackSeekParam.iSpeed = GetSpeed(comboBoxPlaybackSpeed.SelectedIndex);
            pPlaybackSeekParam.iDirection = Convert.ToInt32(comboBoxPlaybackDirection.Text.Substring(0, comboBoxPlaybackDirection.Text.IndexOf("-")));
            DPSDK_RES iRet = DPSDK_PlaybackSeek(m_iSessionID, m_iPlaybackMediaSessionID, ref pPlaybackSeekParam);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "PlaybackSeek success";
            }
            else
            {
                textPlaybackResult.Text = "PlaybackSeek failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackGetPlayedTime_Click(object sender, EventArgs e)
        {
            IntPtr pTime = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_GetPlayedTime(m_iSessionID, m_iPlaybackMediaSessionID, ref pTime);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "GetPlayedTime success, pTime = " + pTime.ToInt64();
            }
            else
            {
                textPlaybackResult.Text = "GetPlayedTime failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackGetProviderType_Click(object sender, EventArgs e)
        {
            IntPtr pProviderType = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_GetProviderType(m_iSessionID, m_iPlaybackMediaSessionID, ref pProviderType);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "GetProviderType success, pProviderType = " + pProviderType.ToInt32();
            }
            else
            {
                textPlaybackResult.Text = "GetProviderType failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackSetPlaybackSpeed_Click(object sender, EventArgs e)
        {
            DPSDK_PLAYBACK_SPEED iSpeed = GetSpeed(comboBoxPlaybackSpeed.SelectedIndex);
            DPSDK_RES iRet = DPSDK_SetPlaybackSpeed(m_iSessionID, m_iPlaybackMediaSessionID, iSpeed);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "SetPlaybackSpeed success";
            }
            else
            {
                textPlaybackResult.Text = "SetPlaybackSpeed failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackDownloadRecordByFile_Click(object sender, EventArgs e)
        {
            if (m_iDownloadMediaSessionID.ToInt32() > 0)
            {
                DPSDK_RES iRet = DPSDK_StopDownloadRecord(m_iSessionID, m_iDownloadMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    textPlaybackResult.Text = "StopDownloadRecord success";
                }
                else
                {
                    textPlaybackResult.Text = "StopDownloadRecord failed, error code = " + iRet.ToString();
                }

                m_iDownloadMediaSessionID = IntPtr.Zero;
                btnPlaybackDownloadRecordByFile.Text = "DownloadRecordByFile";
                btnPlaybackPauseDownloadRecord.Text = "PauseDownloadRecord";
            }
            else
            {
                if (textPlaybackSplitSize.Text == "")
                {
                    AppData.ShowMsgInfo("Please input correct params!");
                    return;
                }
                DPSDK_DOWNLOAD_BY_FILE_PARAM pDownloadByFileParam = new DPSDK_DOWNLOAD_BY_FILE_PARAM();
                if (listViewPlaybackRecordInfo.SelectedIndices.Count > 0)
                {
                    pDownloadByFileParam.szFilename = listViewPlaybackRecordInfo.SelectedItems[0].SubItems[5].Text;
                    pDownloadByFileParam.szFilename = pDownloadByFileParam.szFilename.Replace("efs://", "");
                    pDownloadByFileParam.tBeginTime = Convert.ToInt64(listViewPlaybackRecordInfo.SelectedItems[0].SubItems[3].Text);
                    pDownloadByFileParam.tEndTime = Convert.ToInt64(listViewPlaybackRecordInfo.SelectedItems[0].SubItems[4].Text);
                    pDownloadByFileParam.uSSId = Convert.ToUInt64(listViewPlaybackRecordInfo.SelectedItems[0].SubItems[9].Text);
                    pDownloadByFileParam.uFileHandle = Convert.ToUInt64(listViewPlaybackRecordInfo.SelectedItems[0].SubItems[10].Text);
                    pDownloadByFileParam.szDiskId = listViewPlaybackRecordInfo.SelectedItems[0].SubItems[11].Text;
                }
                else
                {
                    AppData.ShowMsgInfo("Please select one record!");
                    return;
                }
                pDownloadByFileParam.szChannelId = textPlaybackChannelID.Text;
                //pDownloadByFileParam.tBeginTime = Convert.ToInt64(datePlaybackStartTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);
                //pDownloadByFileParam.tEndTime = Convert.ToInt64(datePlaybackEndTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);
                pDownloadByFileParam.iSourceType = (DPSDK_SOURCE_TYPE)Convert.ToInt32(comboBoxPlaybackSourceType.Text.Substring(0, comboBoxPlaybackSourceType.Text.IndexOf("-")));
                pDownloadByFileParam.szChannelName = textPlaybackChannelName.Text;
                pDownloadByFileParam.szDownloadPath = textPlaybackDownloadPath.Text;
                pDownloadByFileParam.iNameRule = (DPSDK_RECORD_FILE_NAME_RULE)Convert.ToInt32(comboBoxPlaybackNameRule.Text.Substring(0, comboBoxPlaybackNameRule.Text.IndexOf("-")));
                pDownloadByFileParam.szDownloadFileName = textPlaybackDownloadFileName.Text;
                pDownloadByFileParam.iSplitFileSize = Convert.ToInt32(textPlaybackSplitSize.Text);
                pDownloadByFileParam.iFileFormat = (DPSDK_DOWNLOAD_RECORD_FILE_FORMAT)Convert.ToInt32(comboBoxPlaybackFileFormat.Text.Substring(0, comboBoxPlaybackFileFormat.Text.IndexOf("-")));
                pDownloadByFileParam.fEventCallBack = m_fEventDownloadCallback;
                DPSDK_RES iRet = DPSDK_StartDownloadRecordByFile(m_iSessionID, ref pDownloadByFileParam, ref m_iDownloadMediaSessionID);

                if (iRet == DPSDK_SUCCESS)
                {
                    btnPlaybackDownloadRecordByFile.Text = "StopDownloadRecord";
                    textPlaybackResult.Text = "StartDownloadRecordByFile success";
                }
                else
                {
                    textPlaybackResult.Text = "StartDownloadRecordByFile failed, error code = " + iRet.ToString();
                }
            }
        }

        private void btnPlaybackDownloadRecordByTime_Click(object sender, EventArgs e)
        {
            if (m_iDownloadMediaSessionID.ToInt32() > 0)
            {
                DPSDK_RES iRet = DPSDK_StopDownloadRecord(m_iSessionID, m_iDownloadMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    textPlaybackResult.Text = "StopDownloadRecord success";
                }
                else
                {
                    textPlaybackResult.Text = "StopDownloadRecord failed, error code = " + iRet.ToString();
                }

                m_iDownloadMediaSessionID = IntPtr.Zero;
                btnPlaybackDownloadRecordByTime.Text = "DownloadRecordByTime";
                btnPlaybackPauseDownloadRecord.Text = "PauseDownloadRecord";
            }
            else
            {
                if (textPlaybackSplitSize.Text == "")
                {
                    AppData.ShowMsgInfo("Please input correct params!");
                    return;
                }
                DPSDK_DOWNLOAD_BY_TIME_PARAM pDownloadByTimeParam = new DPSDK_DOWNLOAD_BY_TIME_PARAM();
                pDownloadByTimeParam.szChannelId = textPlaybackChannelID.Text;
                pDownloadByTimeParam.tBeginTime = Convert.ToInt64(datePlaybackStartTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);
                pDownloadByTimeParam.tEndTime = Convert.ToInt64(datePlaybackEndTime.Value.Subtract(Convert.ToDateTime("1970-1-1 8:00:00")).TotalSeconds);
                pDownloadByTimeParam.iSourceType = (DPSDK_SOURCE_TYPE)Convert.ToInt32(comboBoxPlaybackSourceType.Text.Substring(0, comboBoxPlaybackSourceType.Text.IndexOf("-")));
                pDownloadByTimeParam.iStreamType = (DPSDK_STREAM_TYPE)Convert.ToInt32(comboBoxPlaybackStreamType.Text.Substring(0, comboBoxPlaybackStreamType.Text.IndexOf("-")));
                pDownloadByTimeParam.iRecordType = (DPSDK_RECORD_TYPE)Convert.ToInt32(comboBoxPlaybackQueryRecordType.Text.Substring(0, comboBoxPlaybackRecordType.Text.IndexOf("-")));
                pDownloadByTimeParam.szChannelName = textPlaybackChannelName.Text;
                pDownloadByTimeParam.szDownloadPath = textPlaybackDownloadPath.Text;
                pDownloadByTimeParam.iNameRule = (DPSDK_RECORD_FILE_NAME_RULE)Convert.ToInt32(comboBoxPlaybackNameRule.Text.Substring(0, comboBoxPlaybackNameRule.Text.IndexOf("-")));
                pDownloadByTimeParam.szDownloadFileName = textPlaybackDownloadFileName.Text;
                pDownloadByTimeParam.iSplitFileSize = Convert.ToInt32(textPlaybackSplitSize.Text);
                pDownloadByTimeParam.iFileFormat = (DPSDK_DOWNLOAD_RECORD_FILE_FORMAT)Convert.ToInt32(comboBoxPlaybackFileFormat.Text.Substring(0, comboBoxPlaybackFileFormat.Text.IndexOf("-")));
                pDownloadByTimeParam.fEventCallBack = m_fEventDownloadCallback;
                DPSDK_RES iRet = DPSDK_StartDownloadRecordByTime(m_iSessionID, ref pDownloadByTimeParam, ref m_iDownloadMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    btnPlaybackDownloadRecordByTime.Text = "StopDownloadRecord";
                    textPlaybackResult.Text = "StartDownloadRecordByTime success";
                }
                else
                {
                    textPlaybackResult.Text = "StartDownloadRecordByTime failed, error code = " + iRet.ToString();
                }
            }
        }

        private void btnPlaybackPauseDownloadRecord_Click(object sender, EventArgs e)
        {
            if (btnPlaybackPauseDownloadRecord.Text == "PauseDownloadRecord")
            {
                DPSDK_RES iRet = DPSDK_PauseDownloadRecord(m_iSessionID, m_iDownloadMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    btnPlaybackPauseDownloadRecord.Text = "ResumeDownloadRecord";
                    textPlaybackResult.Text = "PauseDownloadRecord success";
                }
                else
                {
                    textPlaybackResult.Text = "PauseDownloadRecord failed, error code = " + iRet.ToString();
                }
            }
            else
            {
                DPSDK_RES iRet = DPSDK_ResumeDownloadRecord(m_iSessionID, m_iDownloadMediaSessionID);
                if (iRet == DPSDK_SUCCESS)
                {
                    btnPlaybackPauseDownloadRecord.Text = "PauseDownloadRecord";
                    textPlaybackResult.Text = "ResumeDownloadRecord success";
                }
                else
                {
                    textPlaybackResult.Text = "ResumeDownloadRecord failed, error code = " + iRet.ToString();
                }
            }
        }

        private void btnPlaybackGetRecordInfo_Click(object sender, EventArgs e)
        {
            int nFileNum = 100;
            uint uiBufLen = (uint)(Marshal.SizeOf(typeof(DPSDK_DOWNLOAD_RECORD_INFO)) - 8 + DPSDK_FILE_PATH_LEN * nFileNum);
            IntPtr pDownloadInfo = Marshal.AllocHGlobal((int)uiBufLen);
            DPSDK_RES iRet = DPSDK_GetDownloadRecordInfo(m_iSessionID, m_iDownloadMediaSessionID, pDownloadInfo, uiBufLen);
            if (iRet == DPSDK_SUCCESS)
            {
                DPSDK_DOWNLOAD_RECORD_INFO stuDownloadInfo = new DPSDK_DOWNLOAD_RECORD_INFO();
                stuDownloadInfo = (DPSDK_DOWNLOAD_RECORD_INFO)Marshal.PtrToStructure(pDownloadInfo, typeof(DPSDK_DOWNLOAD_RECORD_INFO));
                textPlaybackResult.Text = "GetDownloadRecordInfo success, FileCount = " + stuDownloadInfo.iFileCount.ToString() + ", DownloadStatus = " + stuDownloadInfo.iDownloadStatus.ToString() + ", DownloadMode = " + stuDownloadInfo.iDownloadMode.ToString();
            }
            else
            {
                textPlaybackResult.Text = "GetDownloadRecordInfo failed, error code = " + iRet.ToString();
            }
            Marshal.FreeHGlobal(pDownloadInfo);
        }

        private void btnPlaybackOpenSound_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_OpenSound(m_iSessionID, m_iPlaybackMediaSessionID);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "Open Sound success";
            }
            else
            {
                textPlaybackResult.Text = "Open Sound failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackCloseSound_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_CloseSound(m_iSessionID, m_iPlaybackMediaSessionID);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "Close Sound success";
            }
            else
            {
                textPlaybackResult.Text = "Close Sound failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackOpenShareSound_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_OpenSoundShare(m_iSessionID, m_iPlaybackMediaSessionID);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "Open Sound Share success";
            }
            else
            {
                textPlaybackResult.Text = "Open Sound Share failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackCloseShareSound_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_CloseSoundShare(m_iSessionID, m_iPlaybackMediaSessionID);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "Close Sound Share success";
            }
            else
            {
                textPlaybackResult.Text = "Close Sound Share failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackIsOpenSound_Click(object sender, EventArgs e)
        {
            IntPtr pIsOpenSound = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_IsOpenSoundState(m_iSessionID, m_iPlaybackMediaSessionID, ref pIsOpenSound);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "Is Open Sound State success, bIsOpen = " + pIsOpenSound.ToString();
            }
            else
            {
                textPlaybackResult.Text = "Is Open Sound State failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackSetVolume_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_SetVolume(m_iSessionID, m_iPlaybackMediaSessionID, (uint)trackBarPlaybackVolume.Value);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "Set Volume success";
            }
            else
            {
                textPlaybackResult.Text = "Set Volume failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackGetVolum_Click(object sender, EventArgs e)
        {
            IntPtr pVolume = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_GetVolume(m_iSessionID, m_iPlaybackMediaSessionID, ref pVolume);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "Set Volume success, volum = " + pVolume.ToString();
                labelPlaybackVolume.Text = pVolume.ToString();
                trackBarPlaybackVolume.Value = pVolume.ToInt32();
            }
            else
            {
                textPlaybackResult.Text = "Set Volume failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackGetPictureBuf_Click(object sender, EventArgs e)
        {
            IntPtr pWidth = IntPtr.Zero;
            IntPtr pHeight = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_GetPictureSize(m_iSessionID, m_iPlaybackMediaSessionID, ref pWidth, ref pHeight);
            if (iRet != DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "Get Picture Size failed, error code = " + iRet.ToString();
                return;
            }

            int iBufsize = GetPicBuffSize(comboBoxPlaybackPicFormat.SelectedIndex, pHeight.ToInt32(), pWidth.ToInt32());
            byte[] pPicBuf = new byte[iBufsize];
            IntPtr pPicSize = IntPtr.Zero;
            iRet = DPSDK_GetPictureBuf(m_iSessionID, m_iPlaybackMediaSessionID, ref pPicBuf[0], (IntPtr)iBufsize, ref pPicSize, (uint)comboBoxPlaybackPicFormat.SelectedIndex);
            if (iRet == DPSDK_SUCCESS)
            {
                textPlaybackResult.Text = "Get Picture To Buffer success. BufSize = " + iBufsize.ToString() + ", FactPicSize = " + pPicSize.ToString();
                MemoryStream ms = new MemoryStream(pPicBuf);
                pictureBoxPlaybackPicture.Image = Image.FromStream(ms);

                textPlaybackWidth.Text = pWidth.ToString();
                textPlaybackHeight.Text = pHeight.ToString();
                textPlaybackSize.Text = pPicSize.ToString();
            }
            else
            {
                textPlaybackResult.Text = "Get Picture To Buffer failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackGet24BitPictureFile_Click(object sender, EventArgs e)
        {
            uint uiPicFormat = (uint)comboBoxPlaybackPicFormat.SelectedIndex;
            string strPicturePath = textPlaybackPicPath.Text;
            DPSDK_RES iRet = DPSDK_Get24BitPictureFile(m_iSessionID, m_iPlaybackMediaSessionID, uiPicFormat, strPicturePath);
            if (iRet == DPSDK_SUCCESS)
            {
                pictureBoxPlaybackPicture.Image = Image.FromFile(strPicturePath);
                textPlaybackResult.Text = "Get Picture To File success.";
            }
            else
            {
                textPlaybackResult.Text = "Get Picture To File failed, error code = " + iRet.ToString();
            }
        }

        private void btnPlaybackConvertToJpegFile_Click(object sender, EventArgs e)
        {
            if (m_iConvertType >= 0)
            {
                return;
            }
            m_iConvertType = 1;
        }

        private void btnPlaybackConvertToBmpFile_Click(object sender, EventArgs e)
        {
            if (m_iConvertType >= 0)
            {
                return;
            }
            m_iConvertType = 0;
        }

        private void MediaEventNotify(IntPtr pNotify)
        {
            DPSDK_EVENT_PARAM struNotify = (DPSDK_EVENT_PARAM)Marshal.PtrToStructure(pNotify, typeof(DPSDK_EVENT_PARAM));
            if (struNotify.iSessionID == m_iPlaybackMediaSessionID.ToInt32())
            {
                DPSDK_SCREENSHOT pShotSrc = (DPSDK_SCREENSHOT)Marshal.PtrToStructure(struNotify.pBuf, typeof(DPSDK_SCREENSHOT));
                if (m_iConvertType == 0)
                {
                    DPSDK_CONVERT_BMP pConvertBMP = new DPSDK_CONVERT_BMP();
                    pConvertBMP.pBuf = pShotSrc.pBuf;
                    pConvertBMP.lSize = pShotSrc.lSize;
                    pConvertBMP.lWidth = pShotSrc.lWidth;
                    pConvertBMP.lHeight = pShotSrc.lHeight;
                    pConvertBMP.lType = pShotSrc.lType;
                    pConvertBMP.szFileName = textPlaybackPicPath.Text + DateTime.Now.Hour.ToString() + DateTime.Now.Minute.ToString() + DateTime.Now.Second.ToString() + ".bmp";
                    DPSDK_RES iRet = DPSDK_ConvertToBmpFile(m_iSessionID, m_iPlaybackMediaSessionID, ref pConvertBMP);
                    if (iRet == DPSDK_SUCCESS)
                    {
                        pictureBoxPlaybackPicture.Image = Image.FromFile(pConvertBMP.szFileName);
                        textPlaybackResult.Text = "ConvertToBmpFile success.";
                    }
                    else
                    {
                        textPlaybackResult.Text = "ConvertToBmpFile failed, error code = " + iRet.ToString();
                    }
                    m_iConvertType = -1;
                }
                else if (m_iConvertType == 1)
                {
                    DPSDK_CONVERT_JPEG pConvertJPEG = new DPSDK_CONVERT_JPEG();
                    pConvertJPEG.pBuf = pShotSrc.pBuf;
                    pConvertJPEG.lSize = pShotSrc.lSize;
                    pConvertJPEG.lWidth = pShotSrc.lWidth;
                    pConvertJPEG.lHeight = pShotSrc.lHeight;
                    pConvertJPEG.iQuality = 100;
                    pConvertJPEG.lType = pShotSrc.lType;
                    pConvertJPEG.szFileName = textPlaybackPicPath.Text + DateTime.Now.Hour.ToString() + DateTime.Now.Minute.ToString() + DateTime.Now.Second.ToString() + ".jpg";
                    DPSDK_RES iRet = DPSDK_ConvertToJpegFile(m_iSessionID, m_iPlaybackMediaSessionID, ref pConvertJPEG);
                    if (iRet == DPSDK_SUCCESS)
                    {
                        pictureBoxPlaybackPicture.Image = Image.FromFile(pConvertJPEG.szFileName);
                        textPlaybackResult.Text = "ConvertToJpegFile success.";
                    }
                    else
                    {
                        textPlaybackResult.Text = "ConvertToJpegFile failed, error code = " + iRet.ToString();
                    }
                    m_iConvertType = -1;
                }
            }
        }
    }
}