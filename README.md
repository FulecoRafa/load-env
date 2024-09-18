# Load Env
Simple cli tool to load env files. So you don't `dotenv.load()` in your app :wink:

* [Instalation](#instalation)
* [Usage](#usage)

## Instalation
1) Clone the repository
   ```bash
   git clone https://github.com/fulecorafa/load-env
   cd load-env
   ```
2) Install
   ```bash
   make install
   ```
   Makefile is very simple, installation changes may be added there.

## Usage
Invoke `load-env` with paths to env files, follow by `--` and the command to be run.
Remember that operations like `cmd > file.out` need to be quoted or the output
of `load-env` itself will be captured.

Pass `--confirm` to preview command before runnning.

```bash
# 
> load-env .env -- python3 ./my_script_that_uses_env.py
```
