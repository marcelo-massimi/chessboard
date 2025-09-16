# ♟️ Automated Chessboard Project — Open Source
An open source automated chessboard inspired by Greg06’s instructable. This is a community-driven initiative to build a self-contained electronic chessboard that autonomously senses and moves pieces, and plays against a human using its own algorithm.

Our goal is to deliver a high-quality, affordable, and customizable chessboard that blends modern automation with the timeless strategy of chess — while fostering a collaborative ecosystem of makers, developers, and enthusiasts.

## Project Highlights
🔓 Fully open source: hardware schematics, firmware, and software
🧠 Autonomous gameplay: piece sensing, movement, and AI-based opponent
🛠️ Modular architecture: scalable and maintainable codebase
🌍 Community-driven: contributions, forks, and improvements welcome

## Repository Structure
```
chessboard/
├── docs/                      # Technical documentation and specifications
├── hardware/                  # Hardware-related files and firmware
│   ├── firmware/             # Arduino code and related files
│   └── schematics/          # Electronic diagrams and PCB designs
├── software/                 # Core software components
│   ├── engine/              # Chess engine (Micro_Max) and game logic
│   ├── sensors/             # Hall effect sensors management
│   ├── motion/              # Motors and electromagnet control
│   ├── interface/           # User interface (LCD display, buttons)
│   └── controller/          # Main system orchestration
├── tests/                    # Testing infrastructure
│   ├── unit/               # Unit tests for individual components
│   └── integration/        # End-to-end system tests
├── scripts/                  # Utility scripts and tools
└── README.md                # Project documentation
```
--
## 🧪 Dependencies
- Arduino IDE
- Arduino libraries:
  - Wire
  - LiquidCrystal_I2C

## 🤝 How to Contribute
We welcome pull requests, issues, and ideas. Whether you're improving the AI, refining the motor control, or documenting the system, your input helps the project grow.