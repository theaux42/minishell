# Minishell

💻 **Minishell** is a small version of the Bash shell, written in C. It replicates essential Bash features, enabling users to run commands, manage processes, and interact with their system through a custom command-line interface.

---

## Features

- Execute system commands (e.g. `ls`, `echo`, `cat`)
- Handle command arguments and simple quoting
- Built-in commands: `cd`, `exit`, `env`, `setenv`, `unsetenv`
- Support for pipes (`|`) and redirections (`<`, `>`)
- Environment variable management
- Error handling and user-friendly prompts

---

## How It Works

Minishell reads input from the user, parses the command, and executes it either as a built-in or by spawning a new process. It mimics many fundamental behaviors of Bash, allowing for basic shell scripting and interactive use.

---

## Getting Started

### Prerequisites

- GCC or Clang compiler
- Make
- Unix-like OS (Linux, macOS)

### Build

```bash
git clone https://github.com/theaux42/minishell.git
cd minishell
make
```

### Usage

Run minishell from your terminal:

```bash
./minishell
```

You’ll see a prompt. Enter commands like you would in Bash:

```bash
$ ls -l | grep minishell
$ cd src
$ echo "Hello, Minishell!" > hello.txt
$ cat hello.txt
$ exit
```

---

## Example Commands

- `ls -la`
- `echo Hello World`
- `cat file.txt | grep word`
- `cd ..`
- `export VAR=value`

---

## License

This project is for educational purposes.