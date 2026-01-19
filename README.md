# Complyd Scanner

**Automated Compliance Scanning Tool by Complyd**

Complyd Scanner is a powerful command-line tool designed to automate compliance verification for HIPAA, ISO 27001, and other regulatory frameworks. Built to streamline security audits and compliance checks for modern cloud infrastructures.

## About Complyd

Complyd is a compliance automation company dedicated to simplifying security and regulatory compliance for organizations of all sizes. Our tools help teams achieve and maintain compliance efficiently.

## Features

- ✅ **HIPAA Compliance Checks** - 8 comprehensive security controls
- 📄 **Multiple File Format Support** - JSON, Markdown, YAML, PDF, and text files
- 🔍 **Automated Scanning** - Quick configuration analysis with detailed reports
- 🎯 **Smart Detection** - Intelligent file type detection and parsing
- 📊 **Compliance Scoring** - Clear pass/fail criteria with remediation guidance
- 🚀 **Easy Integration** - Simple CLI interface for CI/CD pipelines

## Supported Compliance Frameworks

- **HIPAA** - Health Insurance Portability and Accountability Act
- **ISO 27001** - Information Security Management (Coming Soon)
- **APPs** - Australian Privacy Principles (Coming Soon)

## Quick Start

### Installation

```bash
# Clone the repository
git clone https://github.com/tphuonglam82/complyd-cli.git
cd complyd-cli

# Build the scanner
make clean && make

# Verify installation
./grc-scan --help
```

### Usage

```bash
# Scan a JSON configuration file
./grc-scan config.json

# Scan a Markdown security policy
./grc-scan security-policy.md

# Scan a YAML configuration
./grc-scan app-config.yaml

# Scan a PDF compliance document
./grc-scan compliance-report.pdf
```

### Example Output

```
   ██████╗ ██████╗ ███╗   ███╗██████╗ ██╗  ██╗   ██╗██████╗
  ██╔════╝██╔═══██╗████╗ ████║██╔══██╗██║  ╚██╗ ██╔╝██╔══██╗
  ██║     ██║   ██║██╔████╔██║██████╔╝██║   ╚████╔╝ ██║  ██║
  ██║     ██║   ██║██║╚██╔╝██║██╔═══╝ ██║    ╚██╔╝  ██║  ██║
  ╚██████╗╚██████╔╝██║ ╚═╝ ██║██║     ███████╗██║   ██████╔╝
   ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝   ╚═════╝

                    Complyd Scanner v1.0

Scanning file: config.json
File type: JSON
✓ Successfully parsed file (1,234 bytes)

HIPAA COMPLIANCE CHECKS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

[PASS] 164.312(a)(2)(iv) - Encryption and Decryption
[PASS] 164.312(b) - Audit Controls
[PASS] 164.312(d) - Person or Entity Authentication
...

Compliance Score: 100.0%
✓ PASSED - Configuration meets HIPAA compliance requirements
```

## HIPAA Compliance Checks

The scanner validates 8 critical HIPAA security controls:

1. **164.312(a)(2)(iv)** - Encryption at Rest
2. **164.312(b)** - Audit Controls
3. **164.312(d)** - Multi-Factor Authentication
4. **164.312(e)(2)(ii)** - Encryption in Transit
5. **164.312(a)(2)(i)** - Unique User Identification
6. **164.308(a)(7)(ii)(A)** - Data Backup
7. **164.308(a)(3)(ii)(C)** - Access Termination
8. **164.312(a)(2)(iii)** - Automatic Logoff

## Supported File Formats

- **JSON** (`.json`) - Structured configuration data
- **Markdown** (`.md`, `.markdown`) - Documentation and policies
- **YAML** (`.yaml`, `.yml`) - Configuration files
- **PDF** (`.pdf`) - Compliance documents
- **Text** (`.txt`, `.conf`, `.config`) - Plain text configurations

## Development

### Building from Source

```bash
# Install dependencies (Ubuntu/Debian)
make install-deps

# Build debug version
make debug

# Build release version
make release

# Run tests
make test
./tests/integration/run_all_tests.sh
```

### Running Tests

```bash
# Run all integration tests
./tests/integration/run_all_tests.sh

# Test specific file formats
./grc-scan tests/fixtures/compliant/config-full-compliant.json
./grc-scan tests/fixtures/compliant/config-full-compliant.md
./grc-scan tests/fixtures/compliant/config-full-compliant.yaml
```

## Project Structure

```
complyd-cli/
├── src/                    # Source code
│   ├── main.c             # Main application
│   ├── frameworks/        # Compliance frameworks
│   │   └── hipaa/        # HIPAA implementation
│   └── parsers/          # File format parsers
├── include/               # Header files
├── tests/                 # Test suite
│   ├── fixtures/         # Test files
│   └── integration/      # Integration tests
├── examples/             # Example configurations
└── Makefile              # Build configuration
```

## CI/CD Integration

### GitHub Actions

```yaml
- name: Run Complyd Scanner
  run: |
    make clean && make
    ./grc-scan config.json
```

### GitLab CI

```yaml
compliance_check:
  script:
    - make clean && make
    - ./grc-scan config.json
```

## Contributing

We welcome contributions! Please see our [Contributing Guidelines](CONTRIBUTING.md) for details.

## Requirements

- GCC compiler
- Make
- libyaml-dev
- pkg-config

## License

Copyright © 2026 Complyd. All rights reserved.

## Support

For questions, issues, or feature requests:
- 📧 Email: support@complyd.com
- 🐛 Issues: [GitHub Issues](https://github.com/tphuonglam82/complyd-cli/issues)
- 📖 Documentation: [docs.complyd.com](https://docs.complyd.com)

## About the Author

Developed and maintained by the Complyd engineering team.

---

**Complyd** - Simplifying Compliance, Securing Your Future
