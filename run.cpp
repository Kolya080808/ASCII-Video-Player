#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <sstream>

namespace fs = std::filesystem;

double speed;
std::vector<std::string> videos;

// Загружаем все кадры в память
std::vector<std::string> load_frames(const std::string &dir) {
    std::vector<std::string> files;
    std::vector<std::string> frames;

    for (auto &p : fs::directory_iterator(dir)) {
        if (p.is_regular_file()) {
            files.push_back(p.path().string());
        }
    }

    std::sort(files.begin(), files.end());

    for (auto &f : files) {
        std::ifstream file(f);
        if (file) {
            std::string content((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
            frames.push_back(content);
        }
    }
    return frames;
}

// Нормализация высоты кадров (чтобы не было разной длины)
std::vector<std::string> normalize_frames(std::vector<std::string> &frames) {
    size_t max_height = 0;
    std::vector<std::vector<std::string>> split_frames;

    for (auto &f : frames) {
        std::vector<std::string> lines;
        std::istringstream iss(f);
        std::string line;
        while (std::getline(iss, line)) {
            lines.push_back(line);
        }
        max_height = std::max(max_height, lines.size());
        split_frames.push_back(std::move(lines));
    }

    std::vector<std::string> fixed;
    for (auto &lines : split_frames) {
        while (lines.size() < max_height) {
            lines.push_back("");
        }
        std::string frame;
        for (auto &l : lines) {
            frame += l + "\n";
        }
        fixed.push_back(frame);
    }

    return fixed;
}

// Проигрывание аудио (mpv в фоне)
void play_audio(const std::string &video) {
    std::string command = "mpv --no-video --audio-device=alsa \""
                        + video + "\" > /dev/null 2>&1";
    std::system(command.c_str());
}

// Отображение ASCII-кадров
void display_frames(const std::vector<std::string> &frames) {
    // Включаем альтернативный экран
    std::cout << "\033[?1049h";
    std::cout.flush();

    for (const auto &frame : frames) {
        std::string out = "\033[H" + frame;  // курсор в начало + кадр
        fwrite(out.c_str(), 1, out.size(), stdout);
        fflush(stdout);
        std::this_thread::sleep_for(std::chrono::duration<double>(speed));
    }

    // Возвращаемся в обычный экран
    std::cout << "\033[?1049l";
    std::cout.flush();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <video_file> [frame_delay]\n";
        return 1;
    }

    videos.push_back(argv[1]);

    if (argc >= 3) {
        try {
            speed = std::stod(argv[2]);
        } catch (...) {
            std::cerr << "Invalid frame delay, using default (0.033)\n";
            speed = 0.033;
        }
    }

    std::vector<std::string> frames = load_frames("frames-ascii");
    frames = normalize_frames(frames);

    std::thread audio_thread(play_audio, videos[0]);
    std::thread video_thread(display_frames, std::cref(frames));

    audio_thread.join();
    video_thread.join();

    return 0;
}
