# DRV8305 Motor Gate Driver IC - Embedded C Driver

**Professional embedded C driver for the Texas Instruments DRV8305-Q1 motor gate driver IC**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C Standard](https://img.shields.io/badge/C-Standard%20C99-blue)](https://en.wikipedia.org/wiki/C99)
[![Platform](https://img.shields.io/badge/DRV8305--Q1-Motor%20Gate%20Driver-brightgreen)](https://www.ti.com/product/DRV8305-Q1#tech-docs)

## 📋 Overview

This project provides a comprehensive, production-ready C driver for the **DRV8305-Q1 three-phase gate driver IC** with advanced motor control capabilities. The driver implements a hierarchical three-tier state machine architecture for managing status monitoring, control register programming, and fault detection.

### Key Features

✅ **Three-Tier Hierarchical State Machine**
- Main state machine for overall control flow
- Status register polling (4 read-only registers)
- Control register programming (7 read/write registers)

✅ **Complete SPI Protocol Implementation**
- Packet formatting for read/write commands
- Response parsing with fault detection
- 16-bit blocking FIFO operations

✅ **Comprehensive Register Management**
- 11 total registers (4 status + 7 control)
- Gate drive control (HS/LS peak current, drive time)
- IC operation (watchdog, undervoltage, faults)
- Shunt amplifier control (gain adjustment)
- Voltage regulator configuration (VREF, UV thresholds)
- VDS sense configuration (thresholds, response modes)

✅ **Hardware Abstraction Layer**
- Callback-based GPIO control (EN_GATE, DRV_WAKE)
- SPI transmit/receive callbacks
- Status and control register callbacks

✅ **Professional Code Quality**
- Comprehensive Doxygen documentation (40+ functions)
- Consistent naming conventions
- Type-safe enum/struct definitions
- MIT License

---

## 📁 Project Structure

```
DRV8305_Repo/
├── DRV8305_API/                          # Core driver API layer
│   ├── drv8305_api.h                     # Public API declarations
│   └── drv8305_api.c                     # State machine implementation
│
├── DRV8305_Config/                       # Configuration module
│   ├── drv8305_configuration.h           # Configuration structure
│   └── drv8305_configuration.c           # Default settings
│
├── DRV8305_Status_Registers/             # Status register handlers
│   ├── drv8305_status_registers_definitions.h
│   ├── drv8305_status_registers_handlers.h
│   └── drv8305_status_registers_handlers.c
│
├── DRV8305_Control_Registers/            # Control register handlers
│   ├── drv8305_control_registers_definitions.h
│   ├── drv8305_control_registers_handlers.h
│   └── drv8305_control_registers_handlers.c
│
├── DRV8305_Driver/                       # Application layer
│   ├── drv8305_app.h                     # Public application interface
│   └── drv8305_app.c                     # Platform implementation
│
├── drv8305_macros.h                      # Global macro definitions
├── drv8305_register_map.h                # Register address constants
├── LICENSE                               # MIT License
└── README.md                             # This file
```

---

## 🏗️ Architecture

### Three-Tier State Machine

The driver uses a hierarchical state machine approach for robust and maintainable control:

```
┌─────────────────────────────────────────────────────────┐
│         MAIN STATE MACHINE (Main Control Flow)          │
├─────────────────────────────────────────────────────────┤
│  States: INIT → IDLE → STATUS ↔ CONTROL ↔ WAKE/SLEEP    │
└──────────────────┬──────────────────────────────────────┘
                   │
        ┌──────────┴──────────┐
        │                     │
    ┌───▼──────────┐    ┌─────▼─────────┐
    │  STATUS SM   │    │  CONTROL SM   │
    ├──────────────┤    ├───────────────┤
    │ 4 Registers: │    │ 7 Registers:  │
    │ • Warning    │    │ • HS Gate     │
    │ • OV/VDS     │    │ • LS Gate     │
    │ • IC Faults  │    │ • Gate Drive  │
    │ • VGS Faults │    │ • IC Operation│
    └──────────────┘    │ • Shunt Amp   │
                        │ • Vol. Reg    │
                        │ • VDS Sense   │
                        └───────────────┘
```

### Register Mapping

**Status Registers (Read-Only via SPI):**
| Register | Address | Purpose | Bits |
|----------|---------|---------|------|
| Warning | 0x01 | Temperature, supply, watchdog | 11 |
| OV/VDS | 0x02 | Overcurrent monitoring | 11 |
| IC Faults | 0x03 | Internal health, thermal | 11 |
| VGS Faults | 0x04 | Gate drive faults | 11 |

**Control Registers (Read/Write via SPI):**
| Register | Address | Purpose | Bits |
|----------|---------|---------|------|
| HS Gate Drive | 0x05 | High-side driver control | 11 |
| LS Gate Drive | 0x06 | Low-side driver control | 11 |
| Gate Drive | 0x07 | Charge pump, PWM mode | 11 |
| IC Operation | 0x09 | Watchdog, undervoltage | 11 |
| Shunt Amplifier | 0x0A | Current sense gain (3 ch) | 11 |
| Voltage Regulator | 0x0B | VREF, sleep delay, UV | 11 |
| VDS Sense | 0x0C | Thresholds, response mode | 11 |

### SPI Protocol

**Write Command Packet Format:**
```
Bit:  15  14-11        10-0
      │    Register    Data
      ├────────────────────┤
R/W=0 │                    │  (16 bits total)
      └────────────────────┘
```

**Read Command Packet Format:**
```
Bit:  15  14-11        10-0
      │    Register    Reserved
      ├────────────────────┤
R/W=1 │                    │  (16 bits total)
      └────────────────────┘
```

**Response Packet Format:**
```
Bit:  15  14-11        10-0
      │    Register    Data
      ├────────────────────┤
Fault │                    │  (16 bits total)
      └────────────────────┘
```

---

## 🚀 Quick Start

### 1. Hardware Setup

**Required Connections:**
- **GPIO Pins:** EN_GATE (enable), DRV_WAKE (sleep control)
- **SPI Interface:** 16-bit SPI (blocking FIFO mode, DRV8305-Q1 compatible)
- **Power:** PVDD (3.3V/5V), VCPH (charge pump), GND

### 2. Minimal Usage Example

```c
#include "drv8305_app.h"

void main(void)
{
    // 1. Initialize driver (must be called once)
    drv8305_initialize();
    
    // 2. Enable driver
    drv8305_motor_run();
    
    // 3. Configure timer interrupt (1ms)
    // Call drv8305_timer() from timer ISR
    
    // 4. Main loop
    while(1)
    {
        // Call periodically from main loop
        drv8305_polling();
        
        // Optional: Check status and faults
        // (handled via callbacks internally)
    }
}

// Timer ISR (1ms)
void TimerISR(void)
{
    drv8305_timer();  // Increment internal counter
}
```

### 3. Initialization Sequence

```
1. drv8305_initialize()         ← Setup hardware callbacks, load config
   ├── Enable GPIO (EN_GATE=1)
   ├── Wake up (DRV_WAKE=1)
   ├── Load default configuration
   └── Initialize state machine
   
2. drv8305_polling()            ← Call repeatedly
   ├── Enter CONTROL state
   ├── Write 7 control registers sequentially
   ├── Transition to IDLE
   └── After 250ms: Read 4 status registers
   
3. drv8305_motor_run()          ← Enable gate drivers
   └── EN_GATE GPIO = 1

4. Operation cycle (repeating)
   ├── 250ms interval: Read status registers
   ├── Process fault/warning callbacks
   └── As needed: Update control registers
```

### 4. Timing Configuration

**Default Timing Constants** (in `drv8305_macros.h`):

```c
#define DRV8305_REGISTER_SWITCH_DELAY_MS    50    // Delay between SPI ops
#define DRV8305_STATUS_POLLING_INTERVAL_MS  250   // Status read interval
```

**Timer Frequency**: Call `drv8305_timer()` every 1ms (typical)
- Timing base for state machine delays
- Maintains accurate polling intervals

---

## 📚 API Reference

### Public Application Functions

#### `drv8305_initialize()`
```c
/**
 * @brief Initialize DRV8305 driver and hardware callbacks
 * @details Sets up driver structures, hardware callbacks, and loads default config
 * @return None
 * @note Must be called once during startup before drv8305_polling()
 */
DRV8305_PUBLIC void drv8305_initialize(void);
```

#### `drv8305_polling()`
```c
/**
 * @brief Execute main state machine polling cycle
 * @details Runs one iteration of the three-tier state machine
 * @return None
 * @note Call repeatedly from main loop
 */
DRV8305_PUBLIC void drv8305_polling(void);
```

#### `drv8305_timer()`
```c
/**
 * @brief Execute timer tick for driver state machine
 * @details Increments internal cycle counter
 * @return None
 * @note Call every 1ms from timer ISR
 */
DRV8305_PUBLIC void drv8305_timer(void);
```

#### `drv8305_motor_run()` / `drv8305_motor_stop()`
```c
/**
 * @brief Enable DRV8305 gate drivers (motor ready)
 * @brief Disable DRV8305 gate drivers (motor stop)
 */
DRV8305_PUBLIC void drv8305_motor_run(void);
DRV8305_PUBLIC void drv8305_motor_stop(void);
```

#### `drv8305_confirm_configuration()`
```c
/**
 * @brief Confirm configuration completion to IC
 * @details Sends acknowledgment after control registers are programmed
 */
DRV8305_PUBLIC void drv8305_confirm_configuration(void);
```

### Public Configuration Functions

#### `drv8305_get_configuration()`
```c
/**
 * @brief Get current DRV8305 configuration
 * @details Retrieves pointer to the active default_configuration structure
 *          containing all current control register settings.
 * @return drv8305_configuration_t* Non-NULL pointer to configuration structure
 * @note Pointer remains valid throughout driver lifetime
 * @usage Modify settings and apply with drv8305_set_configuration()
 * 
 * @example
 * drv8305_configuration_t* config = drv8305_get_configuration();
 * config->hs_gate_drive.peak_current = DRV8305_CTRL05_IPEAK_2_75A;
 * drv8305_set_configuration(config);
 */
DRV8305_PUBLIC drv8305_configuration_t* drv8305_get_configuration(void);
```

#### `drv8305_set_configuration()`
```c
/**
 * @brief Set new DRV8305 configuration
 * @details Copies provided configuration structure into internal default configuration.
 *          Changes take effect after drv8305_api_confirm_configuration() call.
 * @param[in] cfg Pointer to new configuration structure to apply
 * @return None
 * @note Entire configuration is copied (memcpy operation)
 * @warning Configuration changes require drv8305_api_confirm_configuration() to take effect
 * 
 * @example
 * drv8305_configuration_t* config = drv8305_get_configuration();
 * config->gate_drive.pwm_mode = DRV8305_PWM_MODE_PWM_SYNC;
 * drv8305_set_configuration(config);
 * drv8305_api_confirm_configuration();  // Apply changes to IC
 */
DRV8305_PUBLIC void drv8305_set_configuration(drv8305_configuration_t *cfg);
```

---

## 🔧 Configuration

### Configuration Module Location

**File:** `DRV8305_Config/drv8305_configuration.h`  
**Functions:** `drv8305_get_configuration()`, `drv8305_set_configuration()`  
**Header Declarations:** Full Doxygen documentation with usage examples

### Accessing Configuration

The DRV8305 driver provides two main functions for configuration management:

#### `drv8305_get_configuration()`
Returns pointer to current configuration structure. Allows reading and modifying any register parameter.

```c
// Get current configuration
drv8305_configuration_t* config = drv8305_get_configuration();

// Access individual register settings
uint16_t current_peak = config->hs_gate_drive.peak_current;
uint16_t watchdog_time = config->ic_operation.watchdog_time;
```

#### `drv8305_set_configuration()`
Applies new configuration to the driver. Must call `drv8305_api_confirm_configuration()` to send changes to IC.

```c
// Create new configuration
drv8305_configuration_t* config = drv8305_get_configuration();

// Modify settings
config->hs_gate_drive.peak_current = DRV8305_CTRL05_IPEAK_2_75A;
config->gate_drive.pwm_mode = DRV8305_PWM_MODE_PWM_SYNC;
config->ic_operation.watchdog_time = DRV8305_CTRL09_WD_TIME_2ms;

// Apply configuration (sends to IC during next control cycle)
drv8305_set_configuration(config);
drv8305_api_confirm_configuration();  // Must call to apply changes
```

### Complete Configuration Example

```c
// 1. Get current config
drv8305_configuration_t* config = drv8305_get_configuration();

// 2. Modify Gate Drive parameters
config->hs_gate_drive = {
    .peak_current = DRV8305_CTRL05_IPEAK_2_75A,  // 2.75A peak
    .drive_time = DRV8305_CTRL05_TDRV_500ns      // 500ns drive time
};

config->ls_gate_drive = {
    .peak_current = DRV8305_CTRL06_IPEAK_2_75A,
    .drive_time = DRV8305_CTRL06_TDRV_500ns
};

// 3. Modify IC Operation
config->ic_operation = {
    .watchdog_time = DRV8305_CTRL09_WD_TIME_2ms,
    .undervoltage_level = DRV8305_CTRL09_UV_LEVEL_8_0V
};

// 4. Apply configuration
drv8305_set_configuration(config);
drv8305_api_confirm_configuration();
```

### Key Configuration Parameters

**Gate Drive Parameters:**
- Peak source/sink current (HS/LS)
- Drive time (gate rise/fall rate)
- Charge pump frequency

**IC Operation:**
- Watchdog timer threshold (1.1ms - 2.1ms)
- Undervoltage lockout (UV) level
- Fault masks

**Shunt Amplifier:**
- DC offset calibration (per channel)
- Gain selection for 3-phase sensing

**Voltage Regulator:**
- VREF output scaling
- Sleep mode delay
- UV shutdown threshold

---

## ⚠️ Fault & Warning Handling

### Status Register Callbacks

Faults are detected and reported via callback functions during status polling:

```c
// Warning Register (0x01) - Temperature, supply warnings
DRV8305_PRIVATE void drv8305_warning_callback(void *self, uint16_t data)
{
    // Check temperature flags: FLAG1, FLAG2, FLAG3, FLAG4/OTW
    // Check supply faults: PVDD_OVFL, VCPH_OVFL
    // Check watchdog: WD_FLG
}

// OV/VDS Faults (0x02) - Overcurrent detection
DRV8305_PRIVATE void drv8305_ov_vds_callback(void *self, uint16_t data)
{
    // Check shunt overcurrent: SEN_OCP
    // Check MOSFET VDS faults: HA_OC, HB_OC, HC_OC, LA_OC, LB_OC, LC_OC
}

// IC Faults (0x03) - Internal IC health
DRV8305_PRIVATE void drv8305_ic_faults_callback(void *self, uint16_t data)
{
    // Check charge pump: CPUV
    // Check supply: PVDD_UV
    // Check thermal: OTW, OTSD
}

// VGS Faults (0x04) - Gate drive circuit failures
DRV8305_PRIVATE void drv8305_vgs_faults_callback(void *self, uint16_t data)
{
    // Check gate driver faults: HA, HB, HC, LA, LB, LC
}
```

### Fault Response Strategy

| Fault Type | Severity | Action | Recovery |
|-----------|----------|--------|----------|
| Temperature FLAG1 (~105°C) | ⚠️ Warning | Throttle performance | Auto |
| Temperature FLAG4/OTW (~150°C) | 🔴 Critical | Stop immediately | Auto after cool |
| Supply PVDD/VCPH Fault | 🔴 Critical | Stop immediately | Auto after recovery |
| VDS/VGS Fault | 🔴 Critical | Stop immediately | Requires reset |
| Watchdog Timeout | ⚠️ Warning | Log event | Retry |

---

## 📊 Module Documentation

### Core API Layer (`DRV8305_API/`)

**drv8305_api.h / drv8305_api.c**
- Main state machine: `drv8305_api_master_sm_polling()`
- Status polling: `drv8305_status_register_process_polling()`
- Control programming: `drv8305_control_register_process_polling()`
- SPI packet handling: `drv8305_spi_write_packet_create()`, `drv8305_spi_read_packet_create()`
- State transitions: `drv8305_main_sm_go_to_next_state()`

### Configuration Module (`DRV8305_Config/`) ⭐ IMPORTANT

**drv8305_configuration.h / drv8305_configuration.c**

This module manages all DRV8305 register configuration and provides two critical public functions:

- **`drv8305_get_configuration()`** - Get pointer to current configuration structure
  - Returns: `drv8305_configuration_t*` (non-NULL, valid for lifetime of driver)
  - Usage: Read and modify individual register parameters
  - Always available: Call anytime to access/modify settings

- **`drv8305_set_configuration(cfg)`** - Apply new configuration to driver
  - Copies entire configuration structure via memcpy
  - Changes take effect after `drv8305_api_confirm_configuration()` call
  - Usage: Update multiple parameters and apply atomically

**Configuration Structure:**
```
drv8305_configuration_t
├── hs_gate_drive (Register 0x05 parameters)
├── ls_gate_drive (Register 0x06 parameters)
├── gate_drive (Register 0x07 parameters)
├── ic_operation (Register 0x09 parameters)
├── shunt_amplifier (Register 0x0A parameters)
├── voltage_regulator (Register 0x0B parameters)
└── vds_sense (Register 0x0C parameters)
```

**Typical Configuration Flow:**
```
1. drv8305_get_configuration()     ← Access current settings
2. Modify config structure members  ← Change parameters
3. drv8305_set_configuration()     ← Apply changes
4. drv8305_api_confirm_configuration() ← Send to IC
```

**⭐ Quick Find:**
- Header: `DRV8305_Config/drv8305_configuration.h` (lines 52-97)
- Implementation: `DRV8305_Config/drv8305_configuration.c` (lines 144-163)
- Full Doxygen docs with @example tags in header file

### Status Register Handlers (`DRV8305_Status_Registers/`)

**Definitions:** Bit masks and flag definitions (11 bits per register)
**Handlers:** 4 callback functions for each status register
- Warning register (0x01) handler
- OV/VDS faults (0x02) handler
- IC faults (0x03) handler
- VGS faults (0x04) handler

### Control Register Handlers (`DRV8305_Control_Registers/`)

**Definitions:** Enums, structs, and packing macros for 7 control registers
**Handlers:** 7 callback functions for register acknowledgments
- HS Gate Drive (0x05) handler
- LS Gate Drive (0x06) handler
- Gate Drive (0x07) handler
- IC Operation (0x09) handler
- Shunt Amplifier (0x0A) handler
- Voltage Regulator (0x0B) handler
- VDS Sense (0x0C) handler

### Application Layer (`DRV8305_Driver/`)

**drv8305_app.h / drv8305_app.c**
- **Public wrapper functions:** High-level API
- **Hardware callbacks:** GPIO (EN_GATE, DRV_WAKE), SPI (transmit, receive)
- **Status callbacks:** Adapter functions for status handlers
- **Control callbacks:** Adapter functions for control handlers
- **Global user object:** Instantiation and initialization

### Global Definitions

**drv8305_macros.h**
- Visibility modifiers: `DRV8305_PRIVATE`, `DRV8305_PUBLIC`
- Timing: `DRV8305_REGISTER_SWITCH_DELAY_MS`, `DRV8305_STATUS_POLLING_INTERVAL_MS`
- Array indices for 11 registers

**drv8305_register_map.h**
- Register address constants (0x01 - 0x0C)
- Organized by status/control groups

---

## 🛠️ Customization

### Hardware Abstraction & Porting

The driver uses **processor-specific I/O and SPI functions** that must be implemented for your target processor. All hardware operations are isolated in `drv8305_app.c` callback functions.

#### GPIO Operations (Processor-Specific)

The driver requires two GPIO pins controlled via processor functions:

| Function | Processor Library | Purpose |
|----------|-------------------|---------|
| `GPIO_writePin()` | TI C2000 DSP GPIO | Enable/disable gate drivers (EN_GATE) |
| `GPIO_writePin()` | TI C2000 DSP GPIO | Sleep/wake control (DRV_WAKE) |

**Default Implementation (TI C2000):**
```c
// Processor-specific GPIO write function
DRV8305_PRIVATE void hardware_drv8305_io_enable_callback(void)
{
    GPIO_writePin(EN_GATE, 1);  // TI C2000 GPIO API
}
```

**When Porting to Different Processor:**
Replace with your processor's GPIO API:
```c
// Example: ARM Cortex-M4 (STM32)
DRV8305_PRIVATE void hardware_drv8305_io_enable_callback(void)
{
    HAL_GPIO_WritePin(EN_GATE_PORT, EN_GATE_PIN, GPIO_PIN_SET);
}

// Example: Generic GPIO interface
DRV8305_PRIVATE void hardware_drv8305_io_enable_callback(void)
{
    digitalWrite(EN_GATE, HIGH);
}
```

#### SPI Operations (Processor-Specific)

The driver uses **blocking FIFO SPI transfers** via processor-specific functions:

| Function | Processor Library | Purpose |
|----------|-------------------|---------|
| `SPI_writeDataBlockingFIFO()` | TI C2000 DSP SPI | Send 16-bit command packets |
| `SPI_readDataBlockingFIFO()` | TI C2000 DSP SPI | Receive 16-bit response packets |

**Default Implementation (TI C2000 SPIA):**
```c
// Processor-specific SPI write function
DRV8305_PRIVATE void hardware_spi_transmit_callback(uint16_t data)
{
    SPI_writeDataBlockingFIFO(SPIA_BASE, data);  // TI C2000 SPI API
}

// Processor-specific SPI read function
DRV8305_PRIVATE uint16_t hardware_spi_receive_callback(void)
{
    return SPI_readDataBlockingFIFO(SPIA_BASE);  // TI C2000 SPI API
}
```

**When Porting to Different Processor:**
Replace with your processor's SPI API:
```c
// Example: ARM Cortex-M4 (STM32 HAL)
DRV8305_PRIVATE void hardware_spi_transmit_callback(uint16_t data)
{
    HAL_SPI_Transmit(&hspi1, (uint8_t*)&data, 1, HAL_MAX_DELAY);
}

DRV8305_PRIVATE uint16_t hardware_spi_receive_callback(void)
{
    uint16_t rx_data = 0;
    HAL_SPI_Receive(&hspi1, (uint8_t*)&rx_data, 1, HAL_MAX_DELAY);
    return rx_data;
}

// Example: Generic SPI interface
DRV8305_PRIVATE void hardware_spi_transmit_callback(uint16_t data)
{
    spi_write(SPI_BUS, data);
}

DRV8305_PRIVATE uint16_t hardware_spi_receive_callback(void)
{
    return spi_read(SPI_BUS);
}
```

#### Porting Checklist

When porting to a new processor:

1. **Identify GPIO Functions**
   - [ ] Locate processor GPIO write API
   - [ ] Map EN_GATE and DRV_WAKE pins to processor pins
   - [ ] Update `GPIO_writePin()` calls in 4 GPIO callback functions

2. **Identify SPI Functions**
   - [ ] Locate processor SPI transmit API (16-bit blocking)
   - [ ] Locate processor SPI receive API (16-bit blocking)
   - [ ] Identify SPI bus/peripheral (SPIA equivalent)
   - [ ] Update `SPI_writeDataBlockingFIFO()` and `SPI_readDataBlockingFIFO()` calls

3. **Update Pin Definitions**
   - [ ] Define EN_GATE and DRV_WAKE pin constants
   - [ ] Update `#include` statements for processor libraries

4. **Test Hardware Callbacks**
   - [ ] Verify GPIO toggling with multimeter/oscilloscope
   - [ ] Verify SPI communication with bus analyzer
   - [ ] Test callback execution order

#### Hardware Callback Comments

All hardware callback functions include **@processor_specific tags** indicating:
- Which processor library function is used
- Which processor peripheral is involved
- Link to function documentation in processor references

Example from source code:
```c
/**
 * @processor_specific Uses GPIO_writePin() from TI C2000 DSP GPIO library
 * @see GPIO_writePin(), EN_GATE (pin definition in board.h)
 */
```

---

### Modifying Default Configuration

Edit `DRV8305_Config/drv8305_configuration.c`:

```c
DRV8305_PRIVATE drv8305_configuration_t default_configuration = 
{
    .hs_gate_drive = {
        .peak_current = DRV8305_CTRL05_IPEAK_2_75A,
        .drive_time = DRV8305_CTRL05_TDRV_500ns
    },
    .ls_gate_drive = {
        .peak_current = DRV8305_CTRL06_IPEAK_2_75A,
        .drive_time = DRV8305_CTRL06_TDRV_500ns
    },
    // ... more settings
};
```

### Implementing Hardware Callbacks

Edit `DRV8305_Driver/drv8305_app.c` to map to your platform:

```c
// GPIO control example
DRV8305_PRIVATE void hardware_drv8305_io_enable_callback(void)
{
    GPIO_writePin(EN_GATE, 1);  // Your platform's GPIO API
}

// SPI communication example
DRV8305_PRIVATE void hardware_spi_transmit_callback(uint16_t data)
{
    SPI_writeDataBlockingFIFO(SPIA_BASE, data);  // TI C2000 API
}
```

### Adding Custom Fault Handlers

Extend callback functions in `drv8305_app.c`:

```c
DRV8305_PRIVATE void drv8305_warning_callback(void *self, uint16_t data)
{
    // Original handler
    drv8305_warning_register_handler(data);
    
    // Custom logic
    if(data & DRV8305_WARN_OTW) {
        // Handle over-temperature shutdown
        emergency_stop();
    }
}
```

---

## 📖 Documentation Standards

All functions follow Doxygen format:

```c
/**
 * @brief One-line function summary
 * @details Extended description of functionality
 * @param[in] param_name Parameter description
 * @param[in,out] self Object pointer
 * @return Return value description
 * @note Implementation-specific details
 * @see Related_function(), Another_function()
 */
```

Generate HTML documentation:
```bash
doxygen Doxyfile
```

---

## 🧪 Testing Checklist

- [ ] Hardware GPIO lines functional (EN_GATE, DRV_WAKE)
- [ ] SPI bus configured correctly (16-bit, blocking FIFO)
- [ ] `drv8305_initialize()` completes without errors
- [ ] `drv8305_polling()` executes state machine cycle
- [ ] Status registers read correctly (Warning, OV/VDS, etc.)
- [ ] Control registers write successfully
- [ ] Motor enable/disable functions work
- [ ] Fault callbacks triggered on error conditions
- [ ] Temperature warnings detected correctly
- [ ] Watchdog timer resets appropriately

---

## 📝 Version History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | Dec 2025 | Furkan YAYLA | Initial release |

---

## 👤 Author Information

**Furkan YAYLA**  
📧 Email: yaylafurkan@protonmail.com  
📅 Date: December 2025  
📋 License: MIT

---

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2025 Furkan YAYLA

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software...
```

---

## 🔗 References

- **[DRV8305-Q1 Datasheet](https://www.ti.com/product/DRV8305-Q1#tech-docs)** - Official TI Documentation
  - Three-Phase Gate Driver IC with SPI Interface
  - Functional Block Diagram: Page 36
  - Register Map: Pages 37-44
  - Status Registers: Pages 38-39 (Tables 10-13)
  - Control Registers: Pages 40-44 (Tables 14-20)
  - Reset Values: Pages 40-44
  - SPI Communication Protocol: Pages 37-44
  - Application Schematics: Pages 46-50

- **SPI Protocol Implementation**
  - 16-bit word length, blocking FIFO operations
  - Register read/write packet formats
  - Response parsing with fault bit detection

---

## 🐛 Troubleshooting

### Issue: Driver not responding to commands

**Solution:**
1. Verify GPIO pins are properly configured
2. Check SPI bus clock frequency (per DRV8305 datasheet)
3. Confirm hardware callbacks are registered
4. Check PVDD supply voltage

### Issue: Fault flags constantly triggered

**Solution:**
1. Review default configuration for register values
2. Check motor load and temperature
3. Verify gate drive timing (HS/LS TDRV)
4. Monitor supply voltages (PVDD, VCPH)

### Issue: State machine stuck in DELAY state

**Solution:**
1. Ensure `drv8305_timer()` is called periodically
2. Check timer interrupt configuration (1ms rate)
3. Verify timing constants in `drv8305_macros.h`

---

## 📞 Support

For questions or issues regarding this driver:

1. Review the DRV8305-Q1 datasheet
2. Check function Doxygen documentation
3. Examine example implementations in `drv8305_app.c`
4. Verify hardware connections and timing

---

**Last Updated:** December 7, 2025  
**Repository:** pingstech/DRV8305  
**Branch:** main
