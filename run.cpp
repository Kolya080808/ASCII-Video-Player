#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <algorithm>

namespace fs = std::filesystem;

double speed = 1.0;
std::vector<std::string> videos;

void play_audio(const std::string &video) {
    std::string command = std::string("mpv --no-video --audio-device=alsa") 
                        + " \"" + video + "\" > /dev/null 2>&1";
    std::system(command.c_str());
}

void display_output() {
    std::string dir = "frames-ascii";
    std::vector<std::string> files;

    for (auto &p : fs::directory_iterator(dir)) {
        if (p.is_regular_file()) {
            files.push_back(p.path().string());
        }
    }

    std::sort(files.begin(), files.end());
    std::string command_thread = std::string("clear");
    for (auto &f : files) {
        std::ifstream file(f);
        if (file) {
            std::cout << file.rdbuf() << std::flush;
        }
        std::this_thread::sleep_for(std::chrono::duration<double>(speed));
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <video_file> [frame_delay]\n";
        return 1;
    }

    videos.push_back(argv[1]);   // первый аргумент — путь к видео

    if (argc >= 3) {             // если указали второй аргумент
        try {
            speed = std::stod(argv[2]);  // здесь "speed" = задержка кадра
        } catch (...) {
            std::cerr << "Invalid frame delay, using default (0.033)\n";
            speed = 0.033;
        }
    }

    std::thread output_thread(display_output);

    std::vector<std::thread> audio_threads;
    for (auto &video : videos) {
        audio_threads.emplace_back(play_audio, video);
    }

    for (auto &t : audio_threads) {
        t.join();
    }
    output_thread.join();

    return 0;
}

