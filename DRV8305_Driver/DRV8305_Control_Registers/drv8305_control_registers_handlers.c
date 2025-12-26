/**
 * @file drv8305_control_registers_handlers.c
 * @brief DRV8305 Control Register Handlers - Implementation
 * @details Implements handler functions for control register write acknowledgments
 *          and register-specific processing logic.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This module provides control register handlers called after SPI write operations:
 *   - Register 0x05: HS Gate Drive Control handler
 *   - Register 0x06: LS Gate Drive Control handler
 *   - Register 0x07: Gate Drive Control handler
 *   - Register 0x09: IC Operation handler
 *   - Register 0x0A: Shunt Amplifier Control handler
 *   - Register 0x0B: Voltage Regulator Control handler
 *   - Register 0x0C: VDS Sense Control handler
 * 
 * @implementation_notes
 * - Current implementation: placeholder functions (no-op with cast of unused params)
 * - Future use: register-specific processing, diagnostics, logging
 * - All handlers follow same signature: (void *self, uint16_t data)
 * - Unused parameters explicitly cast to (void) to suppress compiler warnings
 */

#include <stdint.h>
#include <stdbool.h>

#include "drv8305_macros.h"
#include "DRV8305_API/drv8305_api.h"
#include "drv8305_control_registers_handlers.h"
#include "drv8305_control_registers_definitions.h"
#include "drv8305_control_registers_handlers.h"


/* --------------------------- CONTROL REGISTERS HANDLERS --------------------------- */

/**
 * @brief Handle DRV8305 Control Register 0x05 (HS Gate Drive Control)
 * @details Processes high-side gate drive configuration acknowledgment including
 *          peak current drive time, sink current, and source current settings.
 * @param[in] self Pointer to DRV8305 user object context\n * @param[in] data Control register 0x05 data bits (echo of written value)\n
 * @return None\n * @note Callback triggered after register write confirmation\n * @see DRV8305_PACK_CTRL05, drv8305_ctrl05_hs_gate_t\n
 * */
DRV8305_PUBLIC void drv8305_hs_gate_drive_register_handler(void *self, uint16_t data)
{
    if(!self) { return; }

    volatile drv8305_user_object_t *user_obj = (drv8305_user_object_t *)self;

    volatile uint16_t tdrive  = (data & DRV8305_CTRL05_CTRL06_TDRIVE_MASK)  >> 8;
    volatile uint16_t isink   = (data & DRV8305_CTRL05_CTRL06_ISINK_MASK)   >> 4;
    volatile uint16_t isource = (data & DRV8305_CTRL05_CTRL06_ISOURCE_MASK) >> 0;

    if (tdrive  == user_obj->config.hs_gate_drive.tdrive &&
        isink   == user_obj->config.hs_gate_drive.isink  &&
        isource == user_obj->config.hs_gate_drive.isource)
    {
        user_obj->configuration_confirmation_flags.hs_gate_drive = true;
    }
    else
    {
        user_obj->configuration_confirmation_flags.hs_gate_drive = false;
    }
}

/**
 * @brief Handle DRV8305 Control Register 0x06 (LS Gate Drive Control)
 * @details Processes low-side gate drive configuration acknowledgment with equivalent
 *          parameters to high-side: peak drive time, sink current, and source current.
 * @param[in] self Pointer to DRV8305 user object context
 * @param[in] data Control register 0x06 data bits (echo of written value)
 * @return None
 * @see DRV8305_PACK_CTRL06, drv8305_ctrl06_ls_gate_t
 */
DRV8305_PUBLIC void drv8305_ls_gate_drive_register_handler(void *self, uint16_t data)
{
    if(!self) { return; }

    volatile drv8305_user_object_t *user_obj = (drv8305_user_object_t *)self;

    volatile uint16_t tdrive  = (data & DRV8305_CTRL05_CTRL06_TDRIVE_MASK)  >> 8;
    volatile uint16_t isink   = (data & DRV8305_CTRL05_CTRL06_ISINK_MASK)   >> 4;
    volatile uint16_t isource = (data & DRV8305_CTRL05_CTRL06_ISOURCE_MASK) >> 0;

    if (tdrive  == user_obj->config.ls_gate_drive.tdrive &&
        isink   == user_obj->config.ls_gate_drive.isink  &&
        isource == user_obj->config.ls_gate_drive.isource)
    {
        user_obj->configuration_confirmation_flags.ls_gate_drive = true;
    }
    else
    {
        user_obj->configuration_confirmation_flags.ls_gate_drive = false;
    }
}

/**
 * @brief Handle DRV8305 Control Register 0x07 (Gate Drive Control)
 * @details Processes general gate drive control settings: charge pump frequency,
 *          commutation mode, PWM input mode, dead time, VDS blanking, and deglitch times.
 * @param[in] self Pointer to DRV8305 user object context
 * @param[in] data Control register 0x07 data bits (echo of written value)
 * @return None
 * @see DRV8305_PACK_CTRL07, drv8305_ctrl07_gate_t
 */
DRV8305_PUBLIC void drv8305_gate_drive_register_handler(void *self, uint16_t data)
{
    if(!self) { return; }

    volatile drv8305_user_object_t *user_obj = (drv8305_user_object_t *)self;

    volatile uint16_t vcph_freq    = (data & DRV8305_CTRL07_VCPH_FREQ_MASK)   >> 10;
    volatile uint16_t comm_option  = (data & DRV8305_CTRL07_COMM_OPTION_MASK) >> 9;
    volatile uint16_t pwm_mode     = (data & DRV8305_CTRL07_PWM_MODE_MASK)    >> 7;
    volatile uint16_t dead_time    = (data & DRV8305_CTRL07_DEAD_TIME_MASK)   >> 4;
    volatile uint16_t tblank       = (data & DRV8305_CTRL07_TBLANK_MASK)      >> 2;
    volatile uint16_t tvds         = (data & DRV8305_CTRL07_TVDS_MASK)        >> 0;    

    if (vcph_freq    == user_obj->config.gate_drive.vcph_freq &&
        comm_option  == user_obj->config.gate_drive.comm_option &&
        pwm_mode     == user_obj->config.gate_drive.pwm_mode &&
        dead_time    == user_obj->config.gate_drive.dead_time &&
        tblank       == user_obj->config.gate_drive.tblank &&
        tvds         == user_obj->config.gate_drive.tvds)
    {
        user_obj->configuration_confirmation_flags.gate_drive = true;
    }
    else
    {
        user_obj->configuration_confirmation_flags.gate_drive = false;
    }
}

/**
 * @brief Handle DRV8305 Control Register 0x09 (IC Operation Control)
 * @details Processes IC operation settings: OTSD flip (DRV8305xE specific), PVDD UVLO
 *          disable, gate driver fault disable, sense clamping, watchdog delay, and modes.
 * @param[in] self Pointer to DRV8305 user object context
 * @param[in] data Control register 0x09 data bits (echo of written value)
 * @return None
 * @see DRV8305_PACK_CTRL09, drv8305_ctrl09_ic_op_t
 */
DRV8305_PUBLIC void drv8305_ic_operation_register_handler(void *self, uint16_t data)
{
    if(!self) { return; }

    volatile drv8305_user_object_t *user_obj = (drv8305_user_object_t *)self;

    volatile uint16_t flip_otsd       = (data & DRV8305_CTRL09_FLIP_OTSD_MASK)      >> 10;
    volatile uint16_t dis_pvdd_uvlo2  = (data & DRV8305_CTRL09_DIS_PVDD_UVLO2_MASK) >> 9;
    volatile uint16_t dis_gdrv_fault  = (data & DRV8305_CTRL09_DIS_GDRV_FAULT_MASK) >> 8;
    volatile uint16_t en_sns_clamp    = (data & DRV8305_CTRL09_EN_SNS_CLAMP_MASK)   >> 7;
    volatile uint16_t wd_dly          = (data & DRV8305_CTRL09_WD_DLY_MASK)         >> 5;
    volatile uint16_t dis_sns_ocp     = (data & DRV8305_CTRL09_DIS_SNS_OCP_MASK)    >> 4;
    volatile uint16_t wd_en           = (data & DRV8305_CTRL09_WD_EN_MASK)          >> 3;
    volatile uint16_t sleep           = (data & DRV8305_CTRL09_SLEEP_MASK)          >> 2;
    volatile uint16_t clr_flts        = (data & DRV8305_CTRL09_CLR_FLTS_MASK)       >> 1;
    volatile uint16_t set_vcph_uv     = (data & DRV8305_CTRL09_SET_VCPH_UV_MASK)    >> 0;   

    if(flip_otsd      == user_obj->config.ic_operation.flip_otsd      &&
       dis_pvdd_uvlo2 == user_obj->config.ic_operation.dis_pvdd_uvlo2 &&
       dis_gdrv_fault == user_obj->config.ic_operation.dis_gdrv_fault &&
       en_sns_clamp   == user_obj->config.ic_operation.en_sns_clamp   &&
       wd_dly         == user_obj->config.ic_operation.wd_dly         &&
       dis_sns_ocp    == user_obj->config.ic_operation.dis_sns_ocp    &&
       wd_en          == user_obj->config.ic_operation.wd_en          &&
       sleep          == user_obj->config.ic_operation.sleep          &&
       //clr_flts       == user_obj->config.ic_operation.clr_flts       &&
       set_vcph_uv    == user_obj->config.ic_operation.set_vcph_uv)
    {
        user_obj->configuration_confirmation_flags.ic_operation = true;
    }
    else
    {
        user_obj->configuration_confirmation_flags.ic_operation = false;
    }   
}

/**
 * @brief Handle DRV8305 Control Register 0x0A (Shunt Amplifier Control)
 * @details Processes shunt amplifier configuration for three current sense channels:
 *          DC calibration mode and gain settings (10 V/V, 20 V/V, 40 V/V, or 80 V/V).
 * @param[in] self Pointer to DRV8305 user object context
 * @param[in] data Control register 0x0A data bits (echo of written value)
 * @return None
 * @see DRV8305_PACK_CTRL0A, drv8305_ctrl0A_shunt_t
 */
DRV8305_PUBLIC void drv8305_shunt_amplifier_register_handler(void *self, uint16_t data)
{
    if(!self) { return; }

    volatile drv8305_user_object_t *user_obj = (drv8305_user_object_t *)self;

    volatile uint16_t dc_cal_ch3  = (data & DRV8305_CTRL0A_DC_CAL_CH3_MASK) >> 10;
    volatile uint16_t dc_cal_ch2  = (data & DRV8305_CTRL0A_DC_CAL_CH2_MASK) >> 9;
    volatile uint16_t dc_cal_ch1  = (data & DRV8305_CTRL0A_DC_CAL_CH1_MASK) >> 8;
    volatile uint16_t cs_blank    = (data & DRV8305_CTRL0A_CS_BLANK_MASK)   >> 6;
    volatile uint16_t gain_cs3    = (data & DRV8305_CTRL0A_GAIN_CH3_MASK)   >> 4;
    volatile uint16_t gain_cs2    = (data & DRV8305_CTRL0A_GAIN_CH2_MASK)   >> 2;
    volatile uint16_t gain_cs1    = (data & DRV8305_CTRL0A_GAIN_CH1_MASK)   >> 0;

    if(dc_cal_ch3  == user_obj->config.shunt_amplifier.dc_cal_ch3 &&
       dc_cal_ch2  == user_obj->config.shunt_amplifier.dc_cal_ch2 &&
       dc_cal_ch1  == user_obj->config.shunt_amplifier.dc_cal_ch1 &&
       cs_blank    == user_obj->config.shunt_amplifier.cs_blank   &&
       gain_cs3    == user_obj->config.shunt_amplifier.gain_cs3   &&
       gain_cs2    == user_obj->config.shunt_amplifier.gain_cs2   &&
       gain_cs1    == user_obj->config.shunt_amplifier.gain_cs1)
    {
        user_obj->configuration_confirmation_flags.shunt_amplifier = true;
    }
    else
    {
        user_obj->configuration_confirmation_flags.shunt_amplifier = false;
    }
}

/**
 * @brief Handle DRV8305 Control Register 0x0B (Voltage Regulator Control)
 * @details Processes internal voltage regulator (VREG) settings: VREF scaling factor,
 *          sleep delay time, power good disable, and undervoltage threshold levels.
 * @param[in] self Pointer to DRV8305 user object context
 * @param[in] data Control register 0x0B data bits (echo of written value)
 * @return None
 * @see DRV8305_PACK_CTRL0B, drv8305_ctrl0B_vreg_t
 */
DRV8305_PUBLIC void drv8305_voltage_regulator_register_handler(void *self, uint16_t data)
{
    if(!self) { return; }

    volatile drv8305_user_object_t *user_obj = (drv8305_user_object_t *)self;

    volatile uint16_t vref_scale      = (data & DRV8305_CTRL0B_VREF_SCALE_MASK)     >> 8;
    volatile uint16_t sleep_dly       = (data & DRV8305_CTRL0B_SLEEP_DELAY_MASK)    >> 3;
    volatile uint16_t dis_vreg_pwrgd  = (data & DRV8305_CTRL0B_DIS_VREG_PWRGD_MASK) >> 2;
    volatile uint16_t vreg_uv_level   = (data & DRV8305_CTRL0B_VREG_UV_LEVEL_MASK)  >> 0;

    if(vref_scale     == user_obj->config.voltage_regulator.vref_scale     &&
       sleep_dly      == user_obj->config.voltage_regulator.sleep_dly      &&
       dis_vreg_pwrgd == user_obj->config.voltage_regulator.dis_vreg_pwrgd &&
       vreg_uv_level  == user_obj->config.voltage_regulator.vreg_uv_level)
    {
        user_obj->configuration_confirmation_flags.voltage_regulator = true;
    }
    else
    {
        user_obj->configuration_confirmation_flags.voltage_regulator = false;
    }
}

/**
 * @brief Handle DRV8305 Control Register 0x0C (VDS Sense Control)
 * @details Processes VDS (drain-source) sense control: threshold level selection
 *          (60 mV to 2.131 V) and response mode (latched shutdown, report-only, disabled).
 * @param[in] self Pointer to DRV8305 user object context
 * @param[in] data Control register 0x0C data bits (echo of written value)
 * @return None
 * @see DRV8305_PACK_CTRL0C, drv8305_ctrl0C_vds_t
 */
DRV8305_PUBLIC void drv8305_vds_sense_register_handler(void *self, uint16_t data)
{
    if(!self) { return; }

    volatile drv8305_user_object_t *user_obj = (drv8305_user_object_t *)self;

    volatile uint16_t vds_level = (data & DRV8305_CTRL0C_VDS_LEVEL_MASK) >> 3;
    volatile uint16_t vds_mode  = (data & DRV8305_CTRL0C_VDS_MODE_MASK)  >> 0;

    if(vds_level == user_obj->config.vds_sense.vds_level &&
       vds_mode  == user_obj->config.vds_sense.vds_mode)
    {
        user_obj->configuration_confirmation_flags.vds_sense = true;
    }
    else
    {
        user_obj->configuration_confirmation_flags.vds_sense = false;
    }
}

