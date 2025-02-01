# Boeing Aircraft Series Avionic Modules with Project Magenta Boeing Suite Avioncs Compatibility (2008)

This project contains schematics and firmware for three Boeing Aircraft Avionic Modules:
- **COMM**
- **NAV**
- **ADF**
- .NET Application which links the hardware modules to the FlightSimulator, which in this instance is (MSFS2004) which in turn links to FSUIPC from Pete Dawson.


I will upload the rest of the project in due course, as I need to seriously carry out some house keeping until it's format is usable for the Person/s interested.


The code includes USB device and host functionality, HID and CDC examples, and display/encoder routines. The firmware has been designed for Microchip PIC microcontrollers using both C18 and C30 compilers. Schematics and additional design materials are available in the project directories.


## Project Structure

- **COMM/** – Contains firmware and source files for the COM module.  
  See [Firmware/COM/main.c](Firmware/COM/main.c) for entry point examples.
- **NAV/** – Contains the firmware for the NAV module, including USB device functionality and various user routines.  
  Key files include [Firmware/NAV/main.c](Firmware/NAV/main.c), [Firmware/NAV/Usb/usb_device.h](Firmware/NAV/Usb/usb_device.h), and [Firmware/NAV/usb_ch9.h](Firmware/NAV/usb_ch9.h).
- **ADF/** – Contains firmware for the ADF module.  
  See [Firmware/ADF/main.c](Firmware/ADF/main.c) and [Firmware/ADF/Usb/usb_host_hid_appl_interface.h](Firmware/ADF/Usb/usb_host_hid_appl_interface.h) for examples.

Other directories like **Software/** provide PC software and plug-and-play support for corresponding modules.

## Getting Started

1. **Toolchain:**  
   - For PIC18 devices, use the Microchip C18 compiler.  
   - For PIC24 devices, use the Microchip C30 compiler.
2. **Hardware:**  
   - The firmware is targeted for devices such as the PICDEM® FS USB Demo Board, PIC18/24 USB PIM modules, and other Boeing Avionic modules.
3. **Building:**  
   - Check the configuration settings in the source (e.g., [Firmware/NAV/main.c](Firmware/NAV/main.c) and [Firmware/ADF/main.c](Firmware/ADF/main.c)) for your target board.
   - Update project settings in your IDE (e.g., MPLAB) as needed.
4. **Programming:**  
   - Follow the provided schematics and design documents to program your selected module.
   - Use the bootloader support if needed (see examples from HID and CDC functionality in the code).

## Usage

- **USB Functionality:**  
  The firmware includes USB host and device routines. For instance, the NAV module uses:
  - USB endpoint definitions in [Firmware/NAV/Usb/usb_device.h](Firmware/NAV/Usb/usb_device.h).
  - USB Chapter 9 specific descriptors in [Firmware/NAV/Usb/usb_ch9.h](Firmware/NAV/Usb/usb_ch9.h).
- **Display Handling:**  
  Display routines for showing data are implemented in the main routines ([Firmware/NAV/main.c](Firmware/NAV/main.c) and [Firmware/ADF/main.c](Firmware/ADF/main.c)).
- **Encoder and Switch Inputs:**  
  The code also manages encoder and switch input handling for user control.
  
Refer to inline comments and function documentation within the code for additional detail on each module.

## License

This project is licensed under the GNU General Public License v3.0.  
See [LICENSE](LICENSE) for more details.


## Acknowledgements

Special thanks to Microchip Technology inc. for the sample firmware and documentation which provided the basis for these modules and a big hello to the entire Flight Sim Community !
