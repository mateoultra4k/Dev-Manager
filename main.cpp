#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <array>
#include <chrono>
#include <regex> // Añadido para limpiar los códigos de color

namespace fs = std::filesystem;
using namespace ftxui;

// Función para limpiar códigos de color ANSI (los [1m[32m que viste)
std::string clean_ansi(std::string text) {
    std::regex ansi_regex("\x1B\\[[0-9;]*[mK]");
    return std::regex_replace(text, ansi_regex, "");
}

const std::vector<std::string> SYSTEM_TOOLS = {
    "rustup", "rustc", "cargo", "rustdoc", "rls", "go", "gofmt", "rustfmt", 
    "cargo-fmt", "cargo-clippy", "cargo-miri", "clippy-driver", "rust-analyzer",
    "rust-gdb", "rust-lldb", "rust-gdbgui"
};

struct BinaryEntry {
    std::string name;
    std::string size;
    std::string full_path;
    std::string last_mod;
    std::string internal_info;
};

std::string exec_cmd(std::string cmd) {
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "No disponible";
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    // Limpiamos los colores antes de devolver la info
    return clean_ansi(result.empty() ? "Sin info extendida" : result);
}

std::string get_file_size(fs::path p) {
    try {
        auto size = fs::file_size(p);
        std::stringstream ss;
        if (size < 1024 * 1024) ss << std::fixed << std::setprecision(1) << (size / 1024.0) << " KB";
        else ss << std::fixed << std::setprecision(1) << (size / (1024.0 * 1024.0)) << " MB";
        return ss.str();
    } catch (...) { return "---"; }
}

std::vector<BinaryEntry> get_filtered_binaries(std::string rel_path, bool is_rust) {
    std::vector<BinaryEntry> bins;
    const char* home = getenv("HOME");
    if (!home) return bins;
    fs::path full_path = fs::path(home) / rel_path.substr(1); 
    
    if (fs::exists(full_path)) {
        for (const auto& entry : fs::directory_iterator(full_path)) {
            if (entry.is_regular_file()) {
                std::string name = entry.path().filename().string();
                if (std::find(SYSTEM_TOOLS.begin(), SYSTEM_TOOLS.end(), name) == SYSTEM_TOOLS.end()) {
                    auto ftime = fs::last_write_time(entry);
                    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                        ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
                    );
                    std::time_t ctime = std::chrono::system_clock::to_time_t(sctp);
                    std::stringstream ss;
                    ss << std::put_time(std::localtime(&ctime), "%Y-%m-%d %H:%M");

                    std::string info;
                    if (is_rust) {
                        info = exec_cmd(entry.path().string() + " --version 2>&1 | head -n 1");
                    } else {
                        info = exec_cmd("go version -m " + entry.path().string() + " 2>&1 | grep 'build' | head -n 1");
                    }
                    bins.push_back({name, get_file_size(entry.path()), entry.path().string(), ss.str(), info});
                }
            }
        }
    }
    std::sort(bins.begin(), bins.end(), [](const BinaryEntry& a, const BinaryEntry& b){ return a.name < b.name; });
    return bins;
}

int main() {
    auto screen = ScreenInteractive::Fullscreen();
    int selected_tab = 0, selected_item = 0;
    bool show_confirm = false, show_about = false, show_details = false;
    std::string status_msg = "Listo.";
    std::vector<BinaryEntry> current_bins;
    std::vector<std::string> menu_labels;

    auto refresh_data = [&]() {
        current_bins = get_filtered_binaries((selected_tab == 0) ? "/.cargo/bin" : "/go/bin", selected_tab == 0);
        menu_labels.clear();
        for (auto& b : current_bins) {
            std::string label = " " + b.name;
            int padding = std::max(2, 45 - (int)b.name.length());
            label += std::string(padding, ' ') + "│ " + b.size;
            menu_labels.push_back(label);
        }
        if (selected_item >= (int)current_bins.size()) selected_item = 0;
    };

    refresh_data();
    auto tab_titles = std::vector<std::string>{" RUST PROJECTS ", " GOLANG PROJECTS "};
    auto tab_menu = Menu(&tab_titles, &selected_tab);
    auto list_menu = Menu(&menu_labels, &selected_item);

    auto renderer = Renderer(Container::Vertical({tab_menu, list_menu}), [&] {
        auto main_ui = vbox({
            hbox({
                text(" 📦 DEV-MANAGER ") | bold | bgcolor(Color::Cyan) | color(Color::Black),
                text("") | color(Color::Cyan) | bgcolor(Color::Blue),
                text(" " + tab_titles[selected_tab] + " ") | color(Color::White) | bgcolor(Color::Blue),
                text("") | color(Color::Blue),
                filler(),
                text("Puerto Montt | 2026 ") | dim,
            }),
            separatorDouble(),
            vbox({
                hbox({ text("  PROYECTO COMPILADO") | bold, filler(), text("TAMAÑO  ") | bold }),
                separator(),
                list_menu->Render() | vscroll_indicator | frame | flex,
            }) | borderRounded | flex | color(Color::LightSteelBlue1),
            hbox({
                text(" [TAB] Switch  [ENTER] Info  [D] Delete  [A] About ") | bgcolor(Color::GrayDark),
                separator(),
                text(" " + status_msg) | color(Color::Yellow),
                filler(),
                text(" [Q] Quit ") | bgcolor(Color::GrayDark),
            }) | color(Color::White)
        }) | flex;

        if (show_details) {
            auto& b = current_bins[selected_item];
            return dbox({ main_ui | dim, center(vbox({
                text(" INSPECCIÓN DE BINARIO ") | hcenter | bold | color(Color::Green),
                separator(),
                hbox(text(" Nombre:    ") | bold, text(b.name) | color(Color::White)),
                hbox(text(" Tamaño:    ") | bold, text(b.size) | color(Color::Yellow)),
                hbox(text(" Compilado: ") | bold, text(b.last_mod) | color(Color::Cyan)),
                separator(),
                text(" INFO DE CONSTRUCCIÓN: ") | bold | dim,
                paragraph(b.internal_info) | color(Color::GrayLight),
                separator(),
                text(" Ruta: ") | bold,
                text(b.full_path) | dim,
                separator(),
                text(" [D] Borrar Aplicación   /   [ENTER] Cerrar ") | hcenter | dim,
            }) | borderHeavy | bgcolor(Color::Black) | size(WIDTH, GREATER_THAN, 80)) });
        }

        if (show_confirm) {
            return dbox({ main_ui | dim, center(vbox({
                text(" ¿ELIMINAR ESTE PROYECTO? ") | hcenter | bold | color(Color::Red),
                text(" " + current_bins[selected_item].name + " ") | hcenter | color(Color::Yellow),
                separator(),
                text(" [S] Confirmar / [N] Cancelar ") | hcenter,
            }) | borderHeavy | bgcolor(Color::Black) | size(WIDTH, GREATER_THAN, 55)) });
        }

        if (show_about) {
            return dbox({ main_ui | dim, center(vbox({
                text(" DEV-BIN-MANAGER ") | hcenter | bold | color(Color::Cyan),
                separator(),
                text("Patricio Rodriguez & Gemini") | hcenter,
                text("Puerto Montt, Chile") | hcenter | dim,
                separator(),
                text(" [A] o [ESC] para cerrar ") | hcenter | dim,
            }) | borderHeavy | bgcolor(Color::Black) | size(WIDTH, GREATER_THAN, 40)) });
        }
        return main_ui;
    });

    auto component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Character('q')) { screen.ExitLoopClosure()(); return true; }
        if (show_about) { if (event == Event::Character('a') || event == Event::Character('A') || event == Event::Escape) show_about = false; return true; }
        if (show_details) {
            if (event == Event::Return || event == Event::Escape) { show_details = false; return true; }
            if (event == Event::Character('d') || event == Event::Character('D')) { show_details = false; show_confirm = true; return true; }
            return true;
        }
        if (show_confirm) {
            if (event == Event::Character('s') || event == Event::Character('S')) {
                fs::remove(current_bins[selected_item].full_path);
                show_confirm = false; refresh_data(); return true;
            }
            if (event == Event::Character('n') || event == Event::Character('N') || event == Event::Escape) { show_confirm = false; return true; }
            return true;
        }
        if (event == Event::Character('a') || event == Event::Character('A')) { show_about = true; return true; }
        if (event == Event::Return) { if (!current_bins.empty()) show_details = true; return true; }
        if (event == Event::Tab) { selected_tab = (selected_tab + 1) % 2; selected_item = 0; refresh_data(); return true; }
        if (event == Event::Character('d') || event == Event::Character('D')) { if (!current_bins.empty()) show_confirm = true; return true; }
        return false;
    });

    screen.Loop(component);
    return 0;
}
