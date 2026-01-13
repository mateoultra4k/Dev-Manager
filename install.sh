#!/bin/bash

# Colores para la terminal
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}--- Instalador de DEV-MANAGER (Patricio Rodriguez) ---${NC}"

# 1. VERIFICACIÓN DE DEPENDENCIAS (Librerías FTXUI)
echo -e "Verificando dependencias del sistema..."
MISSING=()

if [ ! -f /usr/local/lib/libftxui-screen.a ] && [ ! -f /usr/lib/libftxui-screen.so ]; then
    MISSING+=("FTXUI (Librerías de interfaz)")
fi

if ! command -v g++ &> /dev/null; then
    MISSING+=("g++ (Compilador de C++)")
fi

# Si faltan cosas, informar al usuario
if [ ${#MISSING[@]} -ne 0 ]; then
    echo -e "${RED}ERROR: Faltan componentes necesarios para ejecutar el programa:${NC}"
    for item in "${MISSING[@]}"; do
        echo -e "  - $item"
    done
    echo -e "\n${BLUE}¿Cómo obtenerlos?${NC}"
    echo -e "Visita: https://github.com/ArthurSonzogni/FTXUI para instalar la librería visual."
    echo -e "O instala en Debian/Ubuntu: sudo apt install build-essential cmake"
    exit 1
fi

echo -e "${GREEN}✓ Todas las dependencias están presentes.${NC}"

# 2. DESCARGA DEL BINARIO
echo -e "Descargando binario desde GitHub..."
# Reemplaza 'tu-usuario' con tu nombre de usuario real de GitHub
REPO_URL="https://github.com/tu-usuario/dev-manager/raw/main/dev-manager"

curl -L -o dev-manager $REPO_URL

# 3. PERMISOS Y UBICACIÓN
chmod +x dev-manager
sudo mv dev-manager /usr/local/bin/dev-manager

echo -e "${GREEN}--------------------------------------------------${NC}"
echo -e "${GREEN}INSTALACIÓN COMPLETADA EXITOSAMENTE${NC}"
echo -e "Ahora puedes ejecutar el programa escribiendo: ${BLUE}dev-manager${NC}"
echo -e "${GREEN}--------------------------------------------------${NC}"
