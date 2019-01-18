#if defined(__USE_IT6615_TX__)

#ifndef _iTE6615_SYS_FLOW_H_
#define _iTE6615_SYS_FLOW_H_

#define IT6615_DEV			0
#define IT6615_ADR			0x98
#define IT6615_CECADR		0x9C

#define	IT6615_MultiWrite(rAddr, pBuff, bytes)	I2C_Write(TWI_CH0, IT6615_ADR, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes)
#define	IT6615_MultiRead(rAddr, pBuff, bytes)		I2C_Read(TWI_CH0, IT6615_ADR, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes)

// Customer Function
void iTE6615_Customer_Set_InputDualMode(u8 Input_Enable_DualMode, u8 Input_Enable_DualMode_Mode);
void iTE6615_Customer_Set_InputColorDepth(u8 Input_ColorDepth);
void iTE6615_Customer_Set_InputDualMode(u8 Input_Enable_DualMode, u8 Input_Enable_DualMode_Mode);
void iTE6615_Customer_Set_DRM_PRK(u8 Enable);

void iTE6615_fsm(void);

void iTE6615_INIT_Chip(void);
// IRQ
void iTE6615_TX_IRQ(void);
void iTE6615_HDCP_IRQ(void);
void iTE6615_SCDC_IRQ(void);
#if iTE6264
void iTE6615_LVDS_RX_IRQ( void );
#endif

// FSM
void iTE6615_sys_fsm(void);
void iTE6615_sys_chg(iTE6615_STATES_Type NewState );

void iTE6615_vid_fsm(void);
void iTE6615_vid_chg(iTE6615_STATEV_Type NewState);

void iTE6615_aud_fsm(void);
void iTE6615_aud_chg(iTE6615_STATEA_Type NewState);

void iTE6615_hdcp_fsm(void);
void iTE6615_hdcp_chg(STATEH_Type NewState);

// IRQ
void iTE6615_INT_HPD_Chg(void);
void iTE6615_INT_RXSENSE_Chg(void);
void iTE6615_INT_SCDT_Chg(void);
void iTE6615_INT_SCDC_RRDetect( void );
void iTE6615_INT_SCDC_UpdateFlagChg( void );
void iTE6615_INT_SCDC_UpdateFlagChg_Handler( u8 rddata );
void iTE6615_INT_DDC_BusHang( void );

// EDID Parser
u8 iTE6615_EDID_Parser( void );

// output setting
void iTE6615_Set_Video_ColorSpace(void);
void iTE6615_Set_Video_Infoframe(void);
void iTE6615_Set_AVI_Infoframe(void);
void iTE6615_Set_DRM_Infoframe(void);
void iTE6615_Set_VendorSpecific_Infoframe(void);
void iTE6615_Set_HDMI2_SCDC();

#if _ENABLE_6615_SYNC_EMBEDDED_FUNCTIONS_
// Check Embedded
void iTE6615_Check_SyncEmbedded(void);
#endif

// Delay TASK
void iTE6615_TASK_Handler( void );
void iTE6615_TASK_HDCP_PollingBCaps( void );
void iTE6615_TASK_HDCP_RetryAuth( void );
void iTE6615_TASK_HDCP_CheckKSVList( void );
void iTE6615_TASK_AVInfo_SysUnplug( void );
void iTE6615_TASK_AVInfo_ShowAudioInfo(void);
void iTE6615_TASK_AVInfo_ShowVideoInfo( void );

// Identify chip
void iTE6615_Identify_Chip(void);

// mit wrapper function
void ITE6615_Init(void);
void ITE6615ProcHanlder(void);
void ISPM SetITE6615_Tick(void);
void ITE6615_SetFrmt();
void ITE6615_SetAviInfoVIC(BYTE VIC);
void ITE6615_SetAviInfoColor(BYTE Y);
void ITE6615_SetOutColorSpace(BYTE outCSC);
BYTE ITE6615_GetOutColorSpace(void);

#endif


#endif		//__USE_IT6615_TX__

