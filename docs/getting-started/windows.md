# Getting Started on Windows

This guide will help you install and set up Daisy on Windows systems.

## Download Daisy

Download the latest release from [GitHub Releases](https://github.com/daisy-model/daisy/releases/latest).

We provide two distribution options:

=== "Windows Installer (Recommended)"
    - **File**: `daisy-setup.exe` (or similar)
    - **Pros**: Automatic installation, Start menu integration
    - **Cons**: Requires administrator privileges
    - **Installation Path**: Usually `C:\Program Files\daisy 7.1.0`

=== "Zip Archive"
    - **File**: `daisy-windows.zip` (or similar)  
    - **Pros**: No admin rights needed, portable
    - **Cons**: Manual setup required
    - **Installation**: Extract to your preferred location

!!! note "Installation Directory"
    In the following instructions, we refer to `<daisy-dir>`. This is the folder where you install Daisy.
    
    - **Installer**: Usually `C:\Program Files\daisy 7.1.0`
    - **Zip**: Wherever you extracted the files

## Setting up Your Development Environment

While you can run Daisy from the command line, we strongly recommend using a text editor for the best experience.

### Recommended Editors

Choose one of these editors for optimal Daisy development:

- **[Visual Studio Code](../editors/vscode.md)** - Modern, feature-rich editor (recommended)
- **[TextPad](../editors/textpad.md)** - Lightweight, Windows-native editor

Both editors can be configured with:
- Syntax highlighting for `.dai` files
- Integrated Daisy execution
- Error highlighting and navigation

## Running Your First Daisy Program

Daisy includes several sample programs to help you get started.

### Sample Files Location
Sample programs are located in: `<daisy-dir>\sample\`

### Step-by-Step Tutorial

1. **Create a workspace folder**
   - Right-click on your Desktop
   - Select "New" → "Folder"
   - Name it `daisy`

2. **Copy the test file**
   - Navigate to `<daisy-dir>\sample\`
   - Copy `test.dai`
   - Paste it into your Desktop `daisy` folder

3. **Open the file in your editor**
   - Open your configured editor ([VSCode](../editors/vscode.md) or [TextPad](../editors/textpad.md))
   - Open the `test.dai` file from your Desktop `daisy` folder

4. **Run the simulation**
   - Use your editor's run command (typically F5 or Ctrl+R)
   - Or run from command line: `"<daisy-dir>\bin\daisy.exe" test.dai`

### Expected Output Files

After running the simulation, you should see these files in your `daisy` folder:

| File | Description |
|------|-------------|
| `checkpoint-1987-8-7+6.dai` | Simulation state for hot-starting |
| `daisy.log` | Simulation log file |
| `field_nitrogen.dlf` | Field nitrogen data |
| `field_water.dlf` | Field water data |
| `harvest.dlf` | Harvest data |
| `sbarley.dlf` | Spring barley data |
| `soil_nitrogen.dlf` | Soil nitrogen data |
| `soil_water.dlf` | Soil water data |

### Understanding Output Files

The `.dlf` files are tab-separated data files with custom headers that can be:

- **Opened in Excel** or other spreadsheet applications
- **Analyzed with programming languages** (R, Python, etc.)

#### Recommended Analysis Tools

=== "R Users"
    Use RStudio with the [daisyrVis](https://github.com/daisy-model/daisyrVis) package:
    ```r
    install.packages("devtools")
    devtools::install_github("daisy-model/daisyrVis")
    ```

=== "Python Users"
    Use Python with the [daispy-vis](https://github.com/daisy-model/daisypy-vis) package:
    ```bash
    pip install git+https://github.com/daisy-model/daisypy-vis
    ```

=== "Excel Users"
    1. Open Excel
    2. File → Open → Select a `.dlf` file
    3. Choose "Delimited" in the Text Import Wizard
    4. Select "Tab" as the delimiter

## Next Steps

Now that you have Daisy running:

1. **Configure an Editor** - Set up [Visual Studio Code](../editors/vscode.md) or [TextPad](../editors/textpad.md)
2. **Explore More Samples** - Try other example files in the `sample` directory  
3. **Learn the Language** - Study the `.dai` file syntax and structure
4. **Read Documentation** - Visit [daisy.ku.dk](https://daisy.ku.dk/) for detailed model documentation

## Troubleshooting

### Common Issues

??? question "Daisy executable not found"
    **Solution**: Verify your installation path and ensure `daisy.exe` exists in `<daisy-dir>\bin\`

??? question "Permission denied error"
    **Solutions**:
    
    - Run your editor as Administrator
    - Or use the zip distribution instead of the installer
    - Or install to a user directory instead of Program Files

??? question "Sample files not found"
    **Solution**: Check that your Daisy installation is complete and the sample directory exists at `<daisy-dir>\sample\`

??? question "Output files not generated"
    **Possible causes**:
    
    - Check the `daisy.log` file for error messages
    - Ensure you have write permissions in the working directory
    - Verify the `.dai` file syntax is correct

### Command Line Usage

If you prefer command line execution:

```cmd
cd /d "C:\path\to\your\daisy\files"
"<daisy-dir>\bin\daisy.exe" test.dai
```

### Getting Help

If you encounter other issues:

1. Check our [GitHub Issues](https://github.com/daisy-model/daisy/issues)
2. Visit the [official Daisy website](https://daisy.ku.dk/)
3. Review the editor setup guides for configuration help