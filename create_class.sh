#!/bin/bash

# Script to create new C++ class files and update CMakeLists.txt

# Color codes for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Get script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Main loop
while true; do
    clear

    # Prompt for package name
    read -p "Enter package name: " PACKAGE
    if [ -z "$PACKAGE" ]; then
        echo -e "${RED}Error: Package name cannot be empty${NC}"
        read -p "Press Enter to try again..."
        continue
    fi

    # Prompt for class name
    read -p "Enter class name: " CLASS_NAME
    if [ -z "$CLASS_NAME" ]; then
        echo -e "${RED}Error: Class name cannot be empty${NC}"
        read -p "Press Enter to try again..."
        continue
    fi

    # Prompt for cpp file creation (default yes)
    read -p "Create .cpp file? [Y/n]: " CREATE_CPP
    CREATE_CPP=${CREATE_CPP:-Y}
    CREATE_CPP=$(echo "$CREATE_CPP" | tr '[:lower:]' '[:upper:]')

    # Convert class name to lowercase snake_case for file name
    # Remove leading "I" if it's followed by an uppercase letter (interface naming convention)
    FILE_NAME_TEMP="$CLASS_NAME"
    if [[ "$CLASS_NAME" =~ ^I[A-Z] ]]; then
        FILE_NAME_TEMP="${CLASS_NAME:1}"
    fi
    FILE_NAME=$(echo "$FILE_NAME_TEMP" | sed 's/\([A-Z]\)/_\1/g' | tr '[:upper:]' '[:lower:]' | sed 's/^_//')

    # Define paths
    HEADER_DIR="$SCRIPT_DIR/include/$PACKAGE"
    SOURCE_DIR="$SCRIPT_DIR/src/$PACKAGE"
    HEADER_FILE="$HEADER_DIR/${FILE_NAME}.h"
    SOURCE_FILE="$SOURCE_DIR/${FILE_NAME}.cpp"
    CMAKE_FILE="$SCRIPT_DIR/CMakeLists.txt"

    # Create directories if they don't exist
    mkdir -p "$HEADER_DIR"
    mkdir -p "$SOURCE_DIR"

    # Check if files already exist
    if [ -f "$HEADER_FILE" ]; then
        echo -e "${RED}Error: Header file already exists: $HEADER_FILE${NC}"
        read -p "Press Enter to try again..."
        continue
    fi

    if [[ "$CREATE_CPP" == "Y" ]] && [ -f "$SOURCE_FILE" ]; then
        echo -e "${RED}Error: Source file already exists: $SOURCE_FILE${NC}"
        read -p "Press Enter to try again..."
        continue
    fi

    # Create header guard name
    HEADER_GUARD=$(echo "${FILE_NAME}_H" | tr '[:lower:]' '[:upper:]')

    # Create header file with empty class
    cat > "$HEADER_FILE" << EOF
#ifndef ${HEADER_GUARD}
#define ${HEADER_GUARD}

class ${CLASS_NAME} {

};

#endif // ${HEADER_GUARD}
EOF

    echo -e "${GREEN}Created header file: $HEADER_FILE${NC}"

    # Create source file if requested
    if [[ "$CREATE_CPP" == "Y" ]]; then
        cat > "$SOURCE_FILE" << EOF
#include "${PACKAGE}/${FILE_NAME}.h"
EOF
        echo -e "${GREEN}Created source file: $SOURCE_FILE${NC}"
    fi

    # Update CMakeLists.txt
    HEADER_RELATIVE="include/$PACKAGE/${FILE_NAME}.h"
    SOURCE_RELATIVE="src/$PACKAGE/${FILE_NAME}.cpp"

    # Create a temporary file for CMakeLists.txt
    TMP_FILE="${CMAKE_FILE}.tmp"
    cp "$CMAKE_FILE" "$TMP_FILE"

    # Use awk to insert the header in CORE_HEADERS section
    awk -v header="    ${HEADER_RELATIVE}" '
/^set\(CORE_HEADERS/ { in_headers=1 }
in_headers && /^\)/ { print header; in_headers=0 }
{ print }
' "$TMP_FILE" > "${CMAKE_FILE}.new"
    mv "${CMAKE_FILE}.new" "$TMP_FILE"

    # Use awk to insert the source in CORE_SOURCES section only if cpp file was created
    if [[ "$CREATE_CPP" == "Y" ]]; then
        awk -v source="    ${SOURCE_RELATIVE}" '
/^set\(CORE_SOURCES/ { in_sources=1 }
in_sources && /^\)/ { print source; in_sources=0 }
{ print }
' "$TMP_FILE" > "${CMAKE_FILE}.new"
        mv "${CMAKE_FILE}.new" "$TMP_FILE"
    fi

    # Replace original file
    mv "$TMP_FILE" "$CMAKE_FILE"

    echo -e "${GREEN}Updated CMakeLists.txt${NC}"
    echo -e "${BLUE}Summary:${NC}"
    echo -e "  Package: ${PACKAGE}"
    echo -e "  Class: ${CLASS_NAME}"
    echo -e "  Header: ${HEADER_RELATIVE}"
    if [[ "$CREATE_CPP" == "Y" ]]; then
        echo -e "  Source: ${SOURCE_RELATIVE}"
    fi

    echo ""
    read -p "Press Enter to create a new class (or Ctrl+C to exit)..."
done
