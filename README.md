# AUTOSAR CAN Communication Stack for TM4C123

[!\[License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[!\[AUTOSAR](https://img.shields.io/badge/AUTOSAR-4.x-blue.svg)](https://www.autosar.org/)
[!\[Platform](https://img.shields.io/badge/Platform-TM4C123-green.svg)](https://www.ti.com/product/TM4C123GH6PM)

## 📋 Overview

This project implements an AUTOSAR-compliant CAN communication stack for Texas Instruments TM4C123 (Tiva C Series) microcontrollers. It provides a layered architecture following AUTOSAR standards, enabling reliable and standardized CAN communication in automotive embedded systems.

## 🏗️ Architecture

The project follows the AUTOSAR layered architecture:

```
┌─────────────────────────────────────┐
│     Application Layer (COM)         │
├─────────────────────────────────────┤
│     PDU Router (PduR)               │
├─────────────────────────────────────┤
│     CAN Interface (CanIf)           │
├─────────────────────────────────────┤
│     CAN Driver (MCAL)               │
├─────────────────────────────────────┤
│     Hardware (STM32 CAN Controller) │
└─────────────────────────────────────┘
```

### Layer Descriptions

* **COM Layer**: Handles signal-based communication and data management
* **PduR Layer**: Routes Protocol Data Units between different communication modules
* **CanIf Layer**: Provides a uniform interface to CAN driver services
* **MCAL Layer**: Hardware abstraction layer for STM32 CAN controller

## 📁 Project Structure

```
drivers/
├── main.c                      # Application entry point
├── mcal/                       # Microcontroller Abstraction Layer
│   ├── can.c                   # CAN driver implementation
│   ├── can.h                   # CAN driver header
│   ├── can\_stm32.c            # STM32-specific CAN implementation
│   ├── Can\_Cfg.h              # CAN configuration
│   ├── Can\_GeneralTypes.h     # CAN type definitions
│   ├── Can\_IRQ.c              # CAN interrupt handlers
│   ├── Can\_PBCfg.c            # CAN post-build configuration
│   ├── Can\_Reg.h              # CAN register definitions
│   └── common\_macros.h        # Common utility macros
├── Can\_if/                     # CAN Interface Layer
│   ├── CanIf.c                # CanIf implementation
│   ├── CanIf.h                # CanIf header
│   ├── CanIf\_Cbk.c            # CanIf callbacks
│   ├── CanIf\_Cbk.h            # CanIf callback header
│   ├── CanIf\_Cfg.c            # CanIf configuration
│   ├── CanIf\_Cfg.h            # CanIf configuration header
│   └── CanIf\_Types.h          # CanIf type definitions
├── pduR/                       # PDU Router Layer
│   ├── pduR.c                 # PduR implementation
│   ├── PduR.h                 # PduR header
│   ├── PduR\_CanIf.c           # PduR-CanIf interface
│   ├── PduR\_CanIf.h           # PduR-CanIf header
│   ├── PduR\_Cfg.c             # PduR configuration
│   ├── PduR\_Cfg.h             # PduR configuration header
│   ├── PduR\_Com.c             # PduR-COM interface
│   ├── PduR\_Com.h             # PduR-COM header
│   └── PduR\_Types.h           # PduR type definitions
├── com/                        # Communication Layer
│   ├── com.c                  # COM implementation
│   ├── Com.h                  # COM header
│   ├── Com\_Cbk.c              # COM callbacks
│   ├── Com\_Cbk.h              # COM callback header
│   ├── Com\_Cfg.c              # COM configuration
│   ├── Com\_Cfg.h              # COM configuration header
│   └── ComStack\_Types.h       # COM stack type definitions
├── Det/                        # Development Error Tracer
│   ├── Det.c                  # DET implementation
│   └── Det.h                  # DET header
└── Library/                    # Standard AUTOSAR types
    ├── Compiler.h             # Compiler abstraction
    ├── Platform\_Types.h       # Platform-specific types
    └── Std\_Types.h            # Standard AUTOSAR types
```

## ✨ Features

* ✅ **AUTOSAR 4.x Compliant**: Follows AUTOSAR standard specifications
* ✅ **Layered Architecture**: Clean separation of concerns
* ✅ **Standard \& Extended CAN IDs**: Support for both 11-bit and 29-bit identifiers
* ✅ **Hardware Filtering**: Configurable CAN message filtering
* ✅ **Interrupt-Driven**: Efficient interrupt-based communication
* ✅ **Error Detection**: Development Error Tracer (DET) integration
* ✅ **Configurable**: Flexible configuration for different use cases

## 🚀 Getting Started

### Prerequisites

* **Toolchain**: ARM GCC or Keil MDK-ARM
* **Hardware**: STM32F4xx or compatible microcontroller with CAN peripheral
* **Debugger**: ST-Link or J-Link (optional)
* **IDE**: STM32CubeIDE, Keil µVision, or any ARM-compatible IDE

### Hardware Requirements

* TM4C123GH6PM microcontroller (Tiva C Series LaunchPad)
* CAN transceiver (e.g., TJA1050, MCP2551, SN65HVD230)
* CAN bus termination resistors (120Ω)

### Building the Project

1. **Clone the repository**:

```bash
   git clone <repository-url>
   cd drivers
   ```

2. **Configure the project**:

   * Edit [`Can\_Cfg.h`](mcal/Can_Cfg.h) for CAN controller settings
   * Edit [`CanIf\_Cfg.h`](Can_if/CanIf_Cfg.h) for interface configuration
   * Edit [`Com\_Cfg.h`](com/Com_Cfg.h) for communication settings
3. **Build**:

   * Using STM32CubeIDE: Import project and build
   * Using Keil: Open project file and compile
   * Using Makefile: `make all` (if Makefile is configured)
4. **Flash to target**:

```bash
   # Using ST-Link
   st-flash write build/output.bin 0x8000000
   ```

## 📖 Usage Example

```c
#include "can.h"
#include "Com.h"
#include "PduR.h"
#include "CanIf.h"

int main(void)
{
    // Initialize CAN driver
    can\_init(\&Can\_Configuration);
    
    // Initialize CAN Interface
    CanIf\_Init(\&CanIf\_Configuration);
    
    // Initialize PDU Router
    PduR\_Init(\&PduR\_Configuration);
    
    // Initialize COM layer
    Com\_init(\&Com);
    
    // Start CAN controller
    CanIf\_SetControllerMode(CAN0\_CONTROLLER\_ID, CAN\_CS\_STARTED);
    
    // Main loop
    while(1)
    {
        // Application logic here
    }
}
```

## 🔧 Configuration

### CAN Baud Rate Configuration

Edit [`Can\_Cfg.h`](mcal/Can_Cfg.h) to configure baud rate parameters:

```c
typedef struct CanBaudRate
{
    uint16\_t BaudRate;      // Desired baud rate (e.g., 500 kbps)
    uint8\_t PropSeg;        // Propagation segment
    uint8\_t PhaseSeg1;      // Phase segment 1
    uint8\_t PhaseSeg2;      // Phase segment 2
    uint8\_t SyncJumpWidth;  // Synchronization jump width
} CanControllerBaudRateConfig;
```

### Hardware Object Configuration

Configure transmit and receive hardware objects in [`Can\_PBCfg.c`](mcal/Can_PBCfg.c):

```c
CanHardwareObject CanHardwareObject\[] = {
    {
        .CanIdType = ID\_STANDARD,
        .CanObjectType = TRANSMIT,
        .CanHandleType = FULL,
        .CanHardwareObjectCount = 1,
        .CanObjectId = 0x123,
        // ... additional configuration
    }
};
```

## 🧪 Testing

### Unit Testing

* Test individual modules using a unit testing framework
* Mock hardware dependencies for isolated testing

### Integration Testing

* Test communication between layers
* Verify message transmission and reception
* Test error handling scenarios

### Hardware Testing

* Connect two CAN nodes
* Verify message exchange
* Test different baud rates and configurations

## 📊 Performance

* **Latency**: < 1ms for message transmission (depends on baud rate)
* **Throughput**: Up to 1 Mbps (hardware dependent)
* **CPU Usage**: Minimal overhead with interrupt-driven architecture

## 🐛 Debugging

### Enable DET (Development Error Tracer)

The project includes DET for runtime error detection:

```c
#include "Det.h"

// DET will report errors with module ID and error code
// Check Det.c for error logs
```

### Common Issues

1. **CAN bus not working**: Check transceiver connections and termination
2. **No messages received**: Verify hardware filters and acceptance masks
3. **Initialization fails**: Check clock configuration and peripheral enable

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Coding Standards

* Follow AUTOSAR C coding guidelines
* Use consistent naming conventions
* Add comments for complex logic
* Update documentation for new features

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👤 Author

**Muhammed Sahal**

* Created: May 2026
* Email: \[sahalkp33@gmail.com]
* GitHub: https://github.com/Sahalkp33/Autosar-stack/

## 🙏 Acknowledgments

* AUTOSAR consortium for specifications
* Texas Instruments for TM4C123 hardware support
* Open-source community for tools and libraries

## 📚 References

* [AUTOSAR Specification](https://www.autosar.org/standards/)
* [TM4C123GH6PM Datasheet](https://www.ti.com/lit/ds/symlink/tm4c123gh6pm.pdf)
* [Tiva C Series User Guide](https://www.ti.com/lit/ug/spmu296/spmu296.pdf)
* [CAN Bus Protocol](https://en.wikipedia.org/wiki/CAN_bus)



**Note**: This is an educational/demonstration project. For production use, ensure thorough testing and compliance with automotive safety standards (ISO 26262).

