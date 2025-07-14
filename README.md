# 🔐 File Encryption & Decryption in C (RC4)

This project implements a simple file encryption and decryption utility in C using a custom **RC4 stream cipher**, with support for secure key input and random padding.

---

## 📂 Project Structure

```
fileEncry/
├── arcfour.c        # RC4 encryption logic
├── arcfour.h        # RC4 data structures and definitions
├── fileEncry.c      # Encryption tool
├── fileEncry.h      # Encryption headers
├── fsd.c            # Decryption tool
├── fsd.h            # Decryption headers
└── Makefile         # Build configuration
```

---

## 🔧 Build Instructions

### 🔁 Requirements

- GCC (C23 or C2x support recommended)
- Linux system (uses `getrandom`, `termios`, etc.)

### 🛠️ Compile

```bash
make

This will generate two binaries:

    fileEncry – for encryption

    fsd – for decryption

🔒 Usage
🔐 Encrypt a file

./fileEncry <infile> <outfile>

Example:

./fileEncry message.txt encrypted.bin

Youll be prompted to enter a key (input hidden). The file will be:

    Padded with secure random bytes

    Encrypted using RC4 with your key

🔓 Decrypt a file

./fsd <encrypted_file> <output_file>

Example:

./fsd encrypted.bin decrypted.txt

Make sure to use the same key used during encryption.
The program reads the padding size, skips it, and decrypts the remaining bytes.
```
