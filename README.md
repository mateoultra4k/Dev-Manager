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



Ejecuta el siguiente comando en tu terminal:

```bash
curl -sSL [https://raw.githubusercontent.com/mateoultra4k/Dev-Manager/main/install.sh](https://raw.githubusercontent.com/mateoultra4k/Dev-Manager/main/install.sh) | bash
