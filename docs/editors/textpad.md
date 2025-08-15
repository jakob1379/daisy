# Setup TextPad for Daisy

TextPad is a lightweight, Windows-native text editor that provides excellent support for Daisy development. This guide will help you configure TextPad for the best Daisy experience.

!!! info "Windows Only"
    TextPad is a Windows-only editor. For cross-platform alternatives, consider [Visual Studio Code](vscode.md).

## Why TextPad for Daisy?

TextPad offers several advantages for Daisy development:

- **Parenthesis pair matching** - Essential for Lisp-like syntax
- **Syntax highlighting** - Color-coded models, components, and parentheses
- **Integrated execution** - Run Daisy directly from the editor
- **Lightweight** - Fast startup and low resource usage
- **Windows integration** - Native Windows look and feel

## Installation Process

The setup involves three main steps:

1. **Install TextPad and support files**
2. **Create a Daisy document class**
3. **Add Daisy to the Tools menu**

### Step 1: Download and Install

#### Install TextPad

1. **Download TextPad** from [textpad.com](http://www.textpad.com/)
2. **Install TextPad** on your computer
3. **Remember the installation directory** (usually `C:\Program Files\TextPad`)

#### Download Daisy Support Files

1. **Download** the [TextPad Daisy support files](https://daisy.ku.dk/download/windows/textpad/textpad-daisy-v2.0.zip)
2. **Extract the zip file** - you should get these files:
   - `Dai.syn` - Syntax highlighting definition
   - `Daisy.tcl` - Tool configuration
   - `Daisy2.tcl` - Alternative tool configuration  
   - `Daisy3.tcl` - Another tool configuration
3. **Copy all files** to the "Samples" directory under your TextPad installation
   - Usually: `C:\Program Files\TextPad\Samples\`

### Step 2: Create a Daisy Document Class

This step enables syntax highlighting for `.dai` files.

1. **Start TextPad**
2. **Open the Configure menu**
3. **Select "New Document Class..."**
4. **Configure the class**:
   - **Class name**: `Daisy`
   - **Click "Next"**
5. **Set file pattern**:
   - **Class members**: `*.dai`
   - **Click "Next"**
6. **Enable syntax highlighting**:
   - **Check** "Enable syntax highlighting"
   - **Syntax definition file**: Select `Dai.syn` from dropdown
   - **Click "Next"**
7. **Click "Apply"**
8. **Exit TextPad** to make changes take effect

!!! success "Test Syntax Highlighting"
    When you reopen TextPad and load a `.dai` file, you should see syntax highlighting with pretty colors!

### Step 3: Add Daisy to the Tools Menu

This step allows you to run Daisy simulations directly from TextPad.

#### Configure the Tool

1. **Start TextPad**
2. **Open the Configure menu**
3. **Select "Preferences..."**
4. **Click on "Tools"** (the name, not the checkbox)
5. **Add Daisy executable**:
   - **Click "Add"** → **Choose "Program..."**
   - **Navigate to and select** `daisy.exe`
     - Usually in: `C:\Program Files\daisy 7.1.0\bin\daisy.exe`
   - **Click "Apply"**

#### Configure Error Navigation

1. **Expand the Tools section** (click the checkbox next to "Tools")
2. **Select "Daisy"** from the list
3. **Configure error parsing**:
   - **Regular expression**: `([^:\\]+):(\d+):(\d+)`
   - **File dropdown**: Choose `1`
   - **Line dropdown**: Choose `2`  
   - **Column dropdown**: Choose `3`
4. **Click "Apply"**
5. **Exit TextPad** to save changes

!!! tip "Error Navigation"
    With this configuration, you can double-click on error messages in the output to jump directly to the problematic line in your code!

## Using TextPad with Daisy

### Running Simulations

1. **Open a `.dai` file** in TextPad
2. **Run the simulation**:
   - **Menu**: Tools → External Tools → Daisy
   - **Or use keyboard shortcut** (if configured)
3. **View output** in the separate output window
4. **Navigate to errors** by double-clicking error messages

### Keyboard Shortcuts

You can assign keyboard shortcuts to run Daisy:

1. **Open Configure → Preferences**
2. **Go to Keyboard**
3. **Find "Daisy" in the Commands list**
4. **Assign a shortcut** (e.g., F5 or Ctrl+R)

### Working with Multiple Files

TextPad supports:

- **Multiple tabs** - Work on several `.dai` files simultaneously
- **Project management** - Group related files together
- **Find in files** - Search across multiple Daisy files

## Advanced Configuration

### Custom Syntax Highlighting

You can modify `Dai.syn` to customize syntax highlighting:

1. **Open** `C:\Program Files\TextPad\Samples\Dai.syn`
2. **Edit color schemes** and keyword definitions
3. **Save and restart TextPad**

### Multiple Daisy Versions

To support multiple Daisy installations:

1. **Add each version** as a separate tool in Preferences
2. **Give them distinct names** (e.g., "Daisy 7.0.7", "Daisy 7.1.0")
3. **Assign different shortcuts** to each version

## Troubleshooting

### Common Issues

??? question "Syntax highlighting not working"
    **Check**:
    
    - Dai.syn file is in the Samples directory
    - Document class is properly configured
    - File extension is `.dai`
    - TextPad was restarted after configuration

??? question "Daisy tool not found"
    **Verify**:
    
    - Path to daisy.exe is correct
    - Daisy is properly installed
    - You have permissions to execute daisy.exe

??? question "Error navigation not working"
    **Ensure**:
    
    - Regular expression is exactly: `([^:\\]+):(\d+):(\d+)`
    - Dropdown selections are: File=1, Line=2, Column=3
    - Error messages contain file:line:column format

??? question "Output window doesn't appear"
    **Try**:
    
    - Check Windows → Output to show the output window
    - Verify Daisy is producing output
    - Check that the working directory is correct

### File Association Issues

If Windows doesn't recognize `.dai` files:

1. **Right-click a `.dai` file**
2. **Choose "Open with" → "Choose another app"**
3. **Select TextPad**
4. **Check "Always use this app"**

## Comparison with Other Editors

| Feature | TextPad | VSCode |
|---------|---------|---------|
| **Platform** | Windows only | Cross-platform |
| **Resource usage** | Very light | Moderate |
| **Setup complexity** | Moderate | Moderate |
| **Syntax highlighting** | Good | Excellent |
| **Error navigation** | Good | Excellent |
| **Extensibility** | Limited | Extensive |
| **Learning curve** | Easy | Moderate |

## Next Steps

With TextPad configured:

1. **Open sample Daisy files** to test syntax highlighting
2. **Try running a simulation** using the Tools menu
3. **Experiment with error navigation** by introducing syntax errors
4. **Customize shortcuts** and preferences to your liking
5. **Explore TextPad's other features** like macros and clips

## Getting Help

For TextPad-specific issues:

1. Check the [TextPad documentation](http://www.textpad.com/help/)
2. Visit the [TextPad support forums](http://www.textpad.com/support/)
3. For Daisy-specific TextPad issues, check our [GitHub Issues](https://github.com/daisy-model/daisy/issues)