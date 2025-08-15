# Setup Visual Studio Code for Daisy

Visual Studio Code (VSCode) is a modern, feature-rich editor that provides excellent support for Daisy development with syntax highlighting, autocompletion, and integrated execution.

!!! tip "Why VSCode?"
    VSCode offers the best development experience for Daisy with features like:
    
    - Syntax highlighting for `.dai` files
    - Integrated terminal for running simulations
    - Error highlighting and navigation
    - Extensible with plugins
    - Cross-platform support

## Before You Begin

If you already have VSCode installed, ensure that old Daisy-related configurations are removed from your `settings.json` file to avoid conflicts.

### Install VSCode

1. Download VSCode from [code.visualstudio.com](https://code.visualstudio.com/)
2. Install following the platform-specific instructions
3. Launch VSCode

## Enable Syntax Highlighting

Daisy uses a Lisp-like syntax, so we'll configure VSCode to treat `.dai` files as Lisp files.

### Install a Lisp Extension

1. **Open the Extensions view** (Ctrl+Shift+X)
2. **Search for "Lisp"** in the marketplace
3. **Choose one of these options**:

=== "Basic Highlighting"
    Install [Lisp](https://marketplace.visualstudio.com/items?itemName=mattn.Lisp) by mattn
    
    - Provides syntax highlighting only
    - Lightweight and fast
    - Good for basic editing

=== "Advanced Features"
    Install [Common Lisp](https://marketplace.visualstudio.com/items?itemName=qingpeng.common-lisp) by qingpeng
    
    - Syntax highlighting + autocompletion
    - More features but slightly heavier
    - Better for extensive development

### Configure File Association

1. **Open a `.dai` file** in VSCode
2. **Open Command Palette** (F1 or Ctrl+Shift+P)
3. **Type and select** "Change Language Mode"
4. **Select** "Configure File Association for '.dai'..."
5. **Type "lisp"** and select your installed Lisp extension

## Setup VSCode to Run Daisy

There are two complementary approaches for running Daisy from VSCode:

1. **Launch Configuration** - Run with F5 (single Daisy version)
2. **Tasks + Keybindings** - Custom shortcuts (multiple Daisy versions)

### Method 1: Launch Configuration (F5 to Run)

This method is ideal if you have one Daisy installation and want simple F5 execution.

#### Set Default Terminal (Windows Only)

!!! warning "Windows Users"
    For Daisy's spawn program to work correctly, you must use Command Prompt (cmd.exe), not PowerShell.

1. **Open Command Palette** (F1 or Ctrl+Shift+P)
2. **Type and select** "Terminal: Select Default Profile"
3. **Choose** "Command Prompt"

#### Configure the Launch Settings

1. **Open Command Palette** (F1 or Ctrl+Shift+P)
2. **Type and select** "Preferences: Open User Settings (JSON)"
3. **Add this configuration** under the "launch" section:

=== "Windows"
    ```json
    {
        // ... other settings ...
        "launch": {
            "configurations": [
                {
                    "name": "Run Daisy",
                    "request": "launch",
                    "type": "node-terminal",
                    "cwd": "${workspaceFolder}",
                    "command": "\"C:/Program Files/daisy 7.1.0/bin/daisy.exe\" \"${file}\""
                }
            ]
        }
        // ... other settings ...
    }
    ```

=== "MacOS"
    ```json
    {
        // ... other settings ...
        "launch": {
            "configurations": [
                {
                    "name": "Run Daisy",
                    "request": "launch", 
                    "type": "node-terminal",
                    "cwd": "${workspaceFolder}",
                    "command": "\"/Applications/Daisy/bin/daisy\" \"${file}\""
                }
            ]
        }
        // ... other settings ...
    }
    ```

!!! note "Path Adjustment"
    Replace the path in the `command` field with your actual Daisy installation path.

### Method 2: Tasks + Keybindings (Multiple Versions)

This method allows you to switch between multiple Daisy versions using custom keyboard shortcuts.

#### Configure Tasks

1. **Open Command Palette** (F1 or Ctrl+Shift+P)
2. **Type and select** "Tasks: Configure Task"
3. **Select** "Create tasks.json file from template"
4. **Choose** "Others"
5. **Replace the content** with:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "daisy-7.1.0",
            "type": "process",
            "command": "C:/Program Files/daisy 7.1.0/bin/daisy.exe",
            "args": ["${file}"],
            "group": "build",
            "presentation": {
                "echo": true,
                "reveal": "always",
                "focus": false,
                "panel": "new"
            }
        }
    ]
}
```

#### Configure Keybindings

1. **Open Command Palette** (F1 or Ctrl+Shift+P)
2. **Type and select** "Preferences: Open Keyboard Shortcuts (JSON)"
3. **Add this keybinding**:

```json
[
    {
        "key": "ctrl+r 1",
        "command": "workbench.action.tasks.runTask",
        "args": "daisy-7.1.0"
    }
]
```

#### Test the Setup

1. **Open a `.dai` file**
2. **Press and release** Ctrl+R, then press 1
3. **Daisy should run** in the integrated terminal

### Multiple Daisy Versions

To support multiple Daisy installations, add more tasks and keybindings:

=== "tasks.json"
    ```json
    {
        "version": "2.0.0",
        "tasks": [
            {
                "label": "daisy-7.0.7",
                "type": "process",
                "command": "C:/Program Files/daisy 7.0.7/bin/daisy.exe",
                "args": ["${file}"]
            },
            {
                "label": "daisy-7.1.0", 
                "type": "process",
                "command": "C:/Program Files/daisy 7.1.0/bin/daisy.exe",
                "args": ["${file}"]
            }
        ]
    }
    ```

=== "keybindings.json"
    ```json
    [
        {
            "key": "ctrl+r 1",
            "command": "workbench.action.tasks.runTask",
            "args": "daisy-7.0.7"
        },
        {
            "key": "ctrl+r 2", 
            "command": "workbench.action.tasks.runTask",
            "args": "daisy-7.1.0"
        }
    ]
    ```

## Complete Example Configurations

### settings.json
```json
{
    "files.associations": {
        "*.dai": "commonlisp"
    },
    "launch": {
        "configurations": [
            {
                "name": "Run Daisy",
                "request": "launch",
                "type": "node-terminal",
                "command": "\"C:/Program Files/daisy 7.1.0/bin/daisy.exe\" \"${file}\"",
                "cwd": "${workspaceFolder}"
            }
        ]
    },
    "terminal.integrated.defaultProfile.windows": "Command Prompt"
}
```

## Troubleshooting

### Common Issues

??? question "Syntax highlighting not working"
    **Solutions**:
    
    - Verify the Lisp extension is installed and enabled
    - Check that `.dai` files are associated with the Lisp language mode
    - Restart VSCode after making changes

??? question "F5 doesn't run Daisy"
    **Check**:
    
    - Launch configuration is properly added to settings.json
    - File paths in the configuration are correct
    - On Windows, ensure Command Prompt is the default terminal

??? question "Task keybindings not working"
    **Verify**:
    
    - tasks.json exists in `.vscode` folder in your workspace
    - keybindings.json has the correct task names
    - Try the Command Palette: "Tasks: Run Task" to test manually

??? question "Permission denied errors"
    **Solutions**:
    
    - Run VSCode as Administrator (Windows)
    - Check file permissions on the Daisy executable
    - Ensure the working directory is writable

### Getting Help

For VSCode-specific issues:

1. Check the [VSCode documentation](https://code.visualstudio.com/docs)
2. Visit the [VSCode GitHub repository](https://github.com/microsoft/vscode)
3. For Daisy-specific VSCode issues, check our [GitHub Issues](https://github.com/daisy-model/daisy/issues)

## Next Steps

With VSCode configured:

1. **Open a Daisy project folder** in VSCode
2. **Try running a sample file** using F5 or your custom keybindings
3. **Explore the integrated terminal** for command-line operations
4. **Install additional extensions** like Git integration or file explorers
5. **Customize your workspace** with VSCode's extensive settings