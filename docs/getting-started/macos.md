# Getting Started on MacOS

This guide will help you install and set up Daisy on macOS systems.

## Download and Install Daisy

1. Download the latest release of Daisy from [GitHub Releases](https://github.com/daisy-model/daisy/releases/latest)
2. Run the installer package
3. Follow the installation prompts

!!! note "Installation Directory"
    In the following instructions, we refer to `<daisy-dir>`. This is the folder where you install Daisy, which is most likely `/Applications/Daisy`.

## Setup Python on MacOS

!!! warning "Python Requirement"
    To use Python with Daisy on macOS, it is necessary to install Python manually. Daisy will only work with Python 3.13.

### Install Python 3.13

1. Download Python 3.13.2 from the [official Python website](https://www.python.org/ftp/python/3.13.2/python-3.13.2-macos11.pkg)
2. Launch the installer package
3. Follow the installation instructions

## Setting up Your Development Environment

While you can run Daisy from the command line, we recommend using a text editor for a better experience.

### Recommended Editor: TextMate

We recommend using [TextMate](https://macromates.com/) as your editor. The Daisy distribution includes a support bundle for TextMate:

- **Bundle Location**: `<daisy-dir>/TextMate/Daisy.tmbundle`
- **Features**: Syntax highlighting, code completion, and integrated execution

For detailed TextMate setup instructions, see our [editor setup guides](../editors/vscode.md).

## Running Your First Daisy Program

Daisy includes several sample programs to help you get started.

### Sample Files Location
Sample programs are located in: `<daisy-dir>/sample/`

### Step-by-Step Tutorial

1. **Create a workspace folder**
   ```bash
   mkdir ~/Desktop/daisy
   ```

2. **Copy the test file**
   ```bash
   cp "<daisy-dir>/sample/test.dai" ~/Desktop/daisy/
   ```

3. **Open the file in your editor**
   - Navigate to `~/Desktop/daisy/`
   - Open `test.dai` in your preferred editor

4. **Run the simulation**
   - Execute the `test.dai` file using your editor or command line
   - The simulation will process and generate output files

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

- **Opened in spreadsheet applications** (Excel, Numbers, etc.)
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

## Next Steps

Now that you have Daisy running:

1. **Configure an Editor** - Set up [Visual Studio Code](../editors/vscode.md) for enhanced development
2. **Explore More Samples** - Try other example files in the `sample` directory
3. **Learn the Language** - Study the `.dai` file syntax and structure
4. **Read Documentation** - Visit [daisy.ku.dk](https://daisy.ku.dk/) for detailed model documentation

## Troubleshooting

### Common Issues

??? question "Python not found error"
    Make sure you've installed Python 3.13 from python.org, not from Homebrew or other sources.

??? question "Permission denied when running Daisy"
    Check that the Daisy executable has proper permissions:
    ```bash
    chmod +x "<daisy-dir>/bin/daisy"
    ```

??? question "Sample files not found"
    Verify your Daisy installation is complete and the sample directory exists at `<daisy-dir>/sample/`.

### Getting Help

If you encounter other issues:

1. Check our [GitHub Issues](https://github.com/daisy-model/daisy/issues)
2. Visit the [official Daisy website](https://daisy.ku.dk/)
3. Review the [Python setup guide](../python-setup.md) for Python-specific issues