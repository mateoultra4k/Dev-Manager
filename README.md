# 📦 Dev-Manager

**Dev-Manager** es una herramienta de terminal (TUI) ligera y potente diseñada para desarrolladores que trabajan con **Rust** y **Go**. Permite gestionar, inspeccionar y limpiar los binarios compilados en tus carpetas locales de forma rápida y visual.

Desarrollado en **Puerto Montt, Chile**, en colaboración con **Gemini**.

---

## 🚀 Características principales

* **Detección Inteligente:** Identifica automáticamente binarios en `~/.cargo/bin` y `~/go/bin`.
* **Filtro Quirúrgico:** Oculta herramientas del sistema (como `cargo`, `rustc`, `go`) para enfocarse solo en **tus proyectos**.
* **Inspección Profunda (Enter):** Extrae metadatos directamente del binario:
    * Versión y commit (Rust).
    * Build ID y versión del compilador (Go).
    * Fecha exacta de la última compilación.
* **Gestión de Espacio:** Visualiza el tamaño de cada archivo y elimínalos con confirmación visual.
* **Interfaz Moderna:** Construida con **FTXUI**, ofreciendo una experiencia fluida a pantalla completa.

---

## 🛠️ Instalación rápida (Linux)

Puedes instalar **Dev-Manager** en cualquier distribución basada en Debian/Ubuntu (como **elementaryOS**) con una sola línea de comando. El instalador verificará automáticamente si tienes las librerías necesarias.

## 📋 Requisitos del Sistema

Para que el programa funcione correctamente, el instalador verificará la existencia de las siguientes dependencias en tu equipo:

* **FTXUI Libraries:** Las librerías de interfaz gráfica para terminal (necesarias para la visualización de la TUI).
* **G++ / Build Essentials:** Herramientas de compilación para garantizar el soporte de ejecución de **C++17**.

> 💡 **Nota:** Si no cuentas con estos requisitos, el instalador te indicará exactamente qué falta y cómo obtenerlo de forma sencilla.

---

## ⌨️ Atajos de Teclado

| Tecla | Acción |
| :--- | :--- |
| **TAB** | Alternar entre proyectos de **Rust** y **Go** |
| **ENTER** | Ver detalles técnicos e información de construcción del binario |
| **D** | Eliminar el binario seleccionado (incluye ventana de confirmación) |
| **A** | Ver créditos y versión del software (About) |
| **Q** | Salir de la aplicación de forma segura |

Ejecuta el siguiente comando en tu terminal:

```bash
curl -sSL [https://raw.githubusercontent.com/mateoultra4k/Dev-Manager/main/install.sh](https://raw.githubusercontent.com/mateoultra4k/Dev-Manager/main/install.sh) | bash


