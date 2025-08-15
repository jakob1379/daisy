# Daisy User Documentation

This directory contains the user documentation for Daisy, built with [MkDocs](https://www.mkdocs.org/) and the [Material theme](https://squidfunk.github.io/mkdocs-material/).

## Building the Documentation

### Prerequisites

Install the required dependencies:

```bash
pip install mkdocs mkdocs-material
```

Or if you have the project installed:

```bash
pip install -e .
```

### Build Static Site

To build the documentation as static HTML files:

```bash
mkdocs build
```

The built site will be in the `site/` directory.

### Serve Locally

To serve the documentation locally with live reloading:

```bash
mkdocs serve
```

The documentation will be available at `http://localhost:8000`.

### Deploy to GitHub Pages

To deploy to GitHub Pages:

```bash
mkdocs gh-deploy
```

## Documentation Structure

```
docs/
├── index.md                    # Homepage
├── getting-started/
│   ├── overview.md            # Getting started overview
│   ├── macos.md              # MacOS setup guide
│   └── windows.md            # Windows setup guide
├── editors/
│   ├── vscode.md             # Visual Studio Code setup
│   └── textpad.md            # TextPad setup
└── python-setup.md           # Python configuration
```

## Writing Guidelines

- Use clear, user-friendly language suitable for non-developers
- Include step-by-step instructions with screenshots where helpful
- Use admonitions (notes, warnings, tips) to highlight important information
- Organize content with proper headings and navigation
- Test all instructions on the target platforms

## Configuration

The documentation is configured in `mkdocs.yml` with:

- Material theme with green color scheme
- Dark/light mode toggle
- Navigation tabs and sections
- Search functionality
- Syntax highlighting
- Tabbed content support