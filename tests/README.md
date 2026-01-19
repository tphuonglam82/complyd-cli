# HIPAA Compliance Scanner - Test Suite

## Overview
This test suite validates the HIPAA compliance scanner's functionality across different file formats and compliance scenarios.

## Test Structure

```
tests/
├── fixtures/
│   ├── compliant/          # Files that should PASS (100% compliance)
│   └── non_compliant/      # Files that should FAIL (<80% compliance)
├── integration/
│   └── run_all_tests.sh    # Automated test runner
└── README.md               # This file
```

## HIPAA Compliance Checks

The scanner validates 8 HIPAA compliance controls:

1. **164.312(a)(2)(iv)** - Encryption at Rest (HIGH)
2. **164.312(b)** - Audit Controls (HIGH)
3. **164.312(d)** - Multi-Factor Authentication (CRITICAL)
4. **164.312(e)(2)(ii)** - Encryption in Transit (HIGH)
5. **164.312(a)(2)(i)** - Unique User Identification (MEDIUM)
6. **164.308(a)(7)(ii)(A)** - Data Backup (HIGH)
7. **164.308(a)(3)(ii)(C)** - Access Termination (MEDIUM)
8. **164.312(a)(2)(iii)** - Automatic Logoff (LOW)

## Test Files

### Compliant Test Files
These files pass all 8 compliance checks (100% score):
- `config-full-compliant.json` - JSON format
- `config-full-compliant.md` - Markdown format
- `config-full-compliant.yaml` - YAML format

**Expected Result:** Exit code 0, 8/8 checks passed, 100% compliance score

### Non-Compliant Test Files
These files fail specific compliance checks:

#### `no-encryption-at-rest.json`
- **Fails:** Check 1 (Encryption at Rest)
- **Passes:** 7/8 checks
- **Score:** 87.5%
- **Expected Result:** Exit code 1 (under critical threshold)

#### `no-mfa.json`
- **Fails:** Check 3 (MFA Authentication - CRITICAL)
- **Passes:** 7/8 checks
- **Score:** 87.5%
- **Expected Result:** Exit code 1 (critical control failed)

#### `all-failed.json`
- **Fails:** All 8 checks
- **Passes:** 0/8 checks
- **Score:** 0%
- **Expected Result:** Exit code 1

## Running Tests

### Run All Tests
```bash
./tests/integration/run_all_tests.sh
```

### Run Individual Tests
```bash
# Test compliant configuration
./complyd-scan tests/fixtures/compliant/config-full-compliant.json

# Test non-compliant configuration
./complyd-scan tests/fixtures/non_compliant/no-mfa.json

# Test different file formats
./complyd-scan tests/fixtures/compliant/config-full-compliant.md
./complyd-scan tests/fixtures/compliant/config-full-compliant.yaml
```

### Run Examples
```bash
# Test with examples
./complyd-scan examples/json/test-config.json
./complyd-scan examples/markdown/security-config.md
```

## Test Results Interpretation

### Compliant Configuration
```
✓ PASSED - Configuration meets HIPAA compliance requirements
Compliance Score: 100.0%
Exit Code: 0
```

### Non-Compliant Configuration  
```
✗ FAILED - Configuration does not meet HIPAA compliance requirements
Compliance Score: <80.0%
Exit Code: 1
```

## Adding New Tests

### 1. Create Test File
Place your test file in the appropriate directory:
- Compliant: `tests/fixtures/compliant/`
- Non-compliant: `tests/fixtures/non_compliant/`

### 2. Supported Formats
- **JSON** (.json) - Structured data format
- **Markdown** (.md) - Documentation format
- **YAML** (.yaml, .yml) - Configuration format
- **Text** (.txt, .conf) - Plain text format

### 3. Test File Structure
Include the following controls in your configuration:

```json
{
  "encryption": "enabled",
  "encrypt_at_rest": true,
  "kms_key_id": "...",
  "audit_log": "enabled",
  "cloudtrail": "enabled", 
  "logging": true,
  "mfa_enabled": true,
  "require_mfa": true,
  "tls": "enabled",
  "tls_version": "1.3",
  "https_only": true,
  "unique_user_id": true,
  "iam_enabled": true,
  "backup_enabled": true,
  "automated_backup": true,
  "access_termination": "automated",
  "auto_logoff": "enabled",
  "session_timeout": 15
}
```

### 4. Run Tests
```bash
./tests/integration/run_all_tests.sh
```

## CI/CD Integration

Add to your CI pipeline:

```yaml
# GitHub Actions example
- name: Run HIPAA Compliance Tests
  run: |
    make clean && make
    ./tests/integration/run_all_tests.sh
```

## Troubleshooting

### Scanner Not Found
```bash
# Build the scanner
make clean && make
```

### Permission Denied
```bash
# Make test script executable
chmod +x tests/integration/run_all_tests.sh
```

### Test Failures
- Review scanner output for failed checks
- Verify test file contains required keywords
- Check file format is supported
- Ensure configuration includes all 8 controls for compliant tests

## Test Coverage

Current test coverage:
- ✅ Compliant configurations (3 files x 3 formats = 3 files)
- ✅ Non-compliant configurations (3 files)
- ✅ File format parsers (JSON, MD, YAML)
- ✅ Integration test automation

## Support

For issues or questions:
1. Check scanner output for detailed error messages
2. Review remediation suggestions in failed checks
3. Refer to HIPAA compliance documentation
4. Review test file examples

## License

Same as parent project license.
