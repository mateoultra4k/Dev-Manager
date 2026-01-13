#!/bin/bash

# Colores para una interfaz elegante
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' 

echo -e "${BLUE}==============================================${NC}"
echo -e "${GREEN}    INSTALADOR DE DEV-MANAGER v1.0          ${NC}"
echo -e "${BLUE}==============================================${NC}"

# 1. VALIDACIÓN DE DEPENDENCIAS
echo -e "\n${YELLOW}🔍 Verificando librerías del sistema...${NC}"

# Buscamos FTXUI en las rutas estándar de Linux
if [ ! -f /usr/local/lib/libftxui-screen.a ] && [ ! -f /usr/lib/libftxui-screen.so ]; then
    echo -e "${RED}❌ Error: No se encontró la librería FTXUI.${NC}"
    echo -e "Para usar este programa, necesitas instalar FTXUI primero."
    echo -e "Puedes clonarlo de: https://github.com/ArthurSonzogni/FTXUI"
    echo -e "Instrucciones rápidas: mkdir build && cd build && cmake .. && make && sudo make install"
    exit 1
fi

echo -e "${GREEN}✅ Librerías detectadas correctamente.${NC}"

# 2. DESCARGA E INSTALACIÓN
echo -e "\n${YELLOW}📥 Descargando binario desde GitHub...${NC}"

# Ruta directa al archivo binario en tu repo
BINARY_URL="https://github.com/mateoultra4k/Dev-Manager/raw/main/dev-manager"

# Descargamos en una carpeta temporal
curl -L -o /tmp/dev-manager $BINARY_URL

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Falló la descarga. Revisa tu conexión a internet.${NC}"
    exit 1
fi

# 3. PERMISOS Y ACCESO GLOBAL
echo -e "${YELLOW}⚙️  Configurando acceso global...${NC}"
chmod +x /tmp/dev-manager
sudo mv /tmp/dev-manager /usr/local/bin/dev-manager

echo -e "\n${BLUE}==============================================${NC}"
echo -e "${GREEN}🎉 ¡INSTALACIÓN COMPLETADA EXITOSAMENTE!${NC}"
echo -e "Escribe ${YELLOW}dev-manager${GREEN} en tu terminal para comenzar.${NC}"
echo -e "${BLUE}==============================================${NC}"
