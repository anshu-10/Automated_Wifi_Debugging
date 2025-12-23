Self-Recovering Network Infrastructure Using Automated Router Diagnosis
Author

Name: Anshu Jamare
Email: anshujamare10@gmail.com

Overview

This project implements a self-recovering embedded network infrastructure that automatically detects internet connectivity failures and restores network access by rebooting the router without human intervention.

The system continuously monitors network health using multiple verification targets and performs a controlled power-cycle of the router when a genuine failure is detected. The design focuses on reliability, determinism, and commercial feasibility, avoiding OS-dependent solutions.

Problem Statement

Manual router reboots are a common solution to network failures but are:

Time-consuming

Unreliable during unattended hours

Dependent on human intervention

Software or OS-based solutions introduce additional failure points such as crashes, long boot times, or corruption. This project addresses these issues using a hardware-centric, embedded approach.

System Architecture

The system uses a dual-controller architecture:

ESP32

Continuously monitors network connectivity

Performs multi-target ping checks to global DNS servers and local gateway

Determines genuine network failure conditions

Raspberry Pi Pico

Handles deterministic relay control

Executes safe router power-cycle logic

Provides acknowledgement feedback to the ESP32

Relay Module (Opto-isolated)

Safely switches the router power line

LM2596 Buck Converter

Enables a single-plug power solution by deriving stable 5V from the router’s supply

Key Features

Automated detection of network failures

Multi-target ping verification to avoid false positives

Hardware-based router reboot using relay isolation

Controller-to-controller acknowledgement (ACK) mechanism

Single-plug power architecture suitable for commercial deployment

LED-based visual system status indicators

Working Principle

The ESP32 continuously checks network connectivity using multiple endpoints.

A failure is confirmed only if all ping targets fail consecutively.

The ESP32 sends a reboot command to the Raspberry Pi Pico.

The Pico activates the relay to safely power-cycle the router.

After a fixed delay, the router is powered back on.

Normal monitoring resumes automatically.

Applications

Home and office network uptime improvement

ISP-independent router recovery systems

Remote or unattended network installations

Industrial and commercial networking environments

Repository Contents

blink.ino – Embedded control and logic implementation

requirements.txt – Supporting dependencies

Report.pdf – Detailed project documentation

PowerPoint_Presentation.pdf – Project presentation

README.md – Project overview

License

This project is licensed under the MIT License.

You are free to use, modify, and distribute this project with proper attribution to the author.

Contact

For questions, collaboration, or clarification:
Anshu Jamare
📧 anshujamare10@gmail.com