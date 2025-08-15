# Python Setup for Daisy

This guide covers setting up Python to work with Daisy for data analysis and visualization.

!!! info "Python Version Requirement"
    Daisy requires **Python 3.13** for optimal compatibility. Other versions may not work correctly.

## Platform-Specific Setup

=== "MacOS"
    ### Install Python 3.13
    
    1. **Download Python 3.13.2** from the [official Python website](https://www.python.org/ftp/python/3.13.2/python-3.13.2-macos11.pkg)
    2. **Launch the installer** package
    3. **Follow the installation instructions**
    
    !!! warning "Important"
        Do not use Homebrew or other package managers to install Python for Daisy. Use the official python.org installer.
    
    ### Verify Installation
    ```bash
    python3 --version
    # Should output: Python 3.13.2
    ```

=== "Windows"
    ### Install Python 3.13
    
    1. **Download Python 3.13.2** from [python.org](https://www.python.org/downloads/windows/)
    2. **Run the installer**
    3. **Important**: Check "Add Python to PATH" during installation
    4. **Follow the installation wizard**
    
    ### Verify Installation
    ```cmd
    python --version
    # Should output: Python 3.13.2
    ```

=== "Linux"
    ### Install Python 3.13
    
    Most Linux distributions may not have Python 3.13 in their default repositories yet. You may need to:
    
    1. **Use pyenv** (recommended):
    ```bash
    curl https://pyenv.run | bash
    pyenv install 3.13.2
    pyenv global 3.13.2
    ```
    
    2. **Or compile from source** following the [official guide](https://docs.python.org/3/using/unix.html#building-python)

## Installing Analysis Packages

Once Python is installed, you can install packages for analyzing Daisy output files.

### For R Users: daisyrVis

If you prefer R for data analysis:

```r
# Install from R console
install.packages("devtools")
devtools::install_github("daisy-model/daisyrVis")
```

### For Python Users: daispy-vis

If you prefer Python for data analysis:

```bash
# Install the Daisy Python visualization package
pip install git+https://github.com/daisy-model/daisypy-vis
```

### Basic Data Analysis Setup

For general data analysis of Daisy output files:

```bash
# Install common data analysis packages
pip install pandas matplotlib numpy seaborn jupyter
```

## Working with Daisy Output Files

Daisy generates `.dlf` files (Daisy Log Format) which are tab-separated files with custom headers.

### Reading DLF Files in Python

```python
import pandas as pd

# Read a Daisy output file
df = pd.read_csv('field_water.dlf', sep='\t', skiprows=1)
print(df.head())
```

### Basic Visualization Example

```python
import matplotlib.pyplot as plt
import pandas as pd

# Read and plot water content over time
df = pd.read_csv('field_water.dlf', sep='\t', skiprows=1)
plt.figure(figsize=(10, 6))
plt.plot(df['Date'], df['Water_Content'])
plt.title('Soil Water Content Over Time')
plt.xlabel('Date')
plt.ylabel('Water Content')
plt.xticks(rotation=45)
plt.tight_layout()
plt.show()
```

## Jupyter Notebook Setup

For interactive analysis, set up Jupyter notebooks:

```bash
# Install Jupyter
pip install jupyter

# Start Jupyter notebook server
jupyter notebook
```

This will open a web interface where you can create notebooks for analyzing your Daisy simulations.

## Virtual Environments (Recommended)

To avoid conflicts with other Python projects, use virtual environments:

=== "Windows"
    ```cmd
    # Create virtual environment
    python -m venv daisy-env
    
    # Activate it
    daisy-env\Scripts\activate
    
    # Install packages
    pip install pandas matplotlib jupyter
    ```

=== "MacOS/Linux"
    ```bash
    # Create virtual environment
    python3 -m venv daisy-env
    
    # Activate it
    source daisy-env/bin/activate
    
    # Install packages
    pip install pandas matplotlib jupyter
    ```

## Troubleshooting

### Common Issues

??? question "Python not found after installation"
    **Windows**: Make sure you checked "Add Python to PATH" during installation
    
    **MacOS**: You may need to use `python3` instead of `python`
    
    **All platforms**: Restart your terminal/command prompt after installation

??? question "pip install fails with permission errors"
    **Solutions**:
    
    - Use virtual environments (recommended)
    - Add `--user` flag: `pip install --user package-name`
    - On Windows: Run command prompt as Administrator

??? question "Wrong Python version being used"
    **Check your PATH**: Multiple Python installations can conflict
    
    ```bash
    # Check which Python is being used
    which python3  # MacOS/Linux
    where python   # Windows
    ```

??? question "Package import errors"
    **Verify installation**:
    ```python
    import sys
    print(sys.version)  # Check Python version
    print(sys.path)     # Check package search paths
    ```

### Getting Help

For Python-specific issues:

1. Check the [Python documentation](https://docs.python.org/3.13/)
2. Visit [Stack Overflow](https://stackoverflow.com/questions/tagged/python) for common problems
3. For Daisy-specific Python issues, check our [GitHub Issues](https://github.com/daisy-model/daisy/issues)

## Next Steps

With Python set up, you can:

1. **Analyze simulation results** using pandas and matplotlib
2. **Create visualizations** of your Daisy model outputs  
3. **Automate analysis workflows** with Python scripts
4. **Use Jupyter notebooks** for interactive exploration
5. **Integrate with other tools** in the Python ecosystem