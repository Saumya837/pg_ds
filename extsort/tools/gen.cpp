#include <charconv>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

struct Record{
    std::optional<std::string> mode;
    std::optional<int> bytes;
    std::optional<int> seed;
    std::optional<std::string> out_file;
};

static bool is_flag(const std::string_view arg){ return arg.starts_with("--"); }

 static std::optional<int> parse_int(const std::string_view s){
    int n{};

    const char*  first = s.data();
    const char*  last = s.data() + s.size();

    auto [end, rc] = std::from_chars(first, last, n);
    if (rc != std::errc{} || end != last) return std::nullopt;

    return n;
}

static std::optional<Record> parse_args(int argc, char** argv){
    Record record;
    for (int i = 1; i < argc; ++i){
        std::string_view flag = argv[i];
        if(!is_flag(flag)){
            std::cerr << "unexpected argument:" << flag <<"\n";
            return std::nullopt;
        }
        if(i+1 >= argc || is_flag(argv[i+1])){
            std::cerr << flag << " requires a value\n";
            return std::nullopt;
        }
        std::string_view value = argv[++i];
        if(flag == "--mode") record.mode = std::string(value);
        else if(flag == "--bytes") {
            auto n = parse_int(value);
            if (!n || *n < 0) {
                std::cerr << "--bytes: invalid number '" << value << "'\n";
                return std::nullopt;
            }
            record.bytes = *n;
        }
        else if(flag == "--seed") {
            auto n = parse_int(value);
            if (!n || *n < 0) {
                std::cerr << "--seed: invalid number '" << value << "'\n";
                return std::nullopt;
            }
            record.seed = *n;
        }
        else if(flag == "--out") {
            record.out_file = std::string(value);
        }
        else {
            std::cerr << "unknown option: " << flag << '\n';
            return std::nullopt;
        }
    }
    return record;
}

int main(int argc, char** argv){
    auto opt = parse_args(argc, argv);
    if (!opt) return 1;

    if (opt->mode)    std::cout << "Mode: "    << *opt->mode    << '\n';
    if (opt->bytes) std::cout << "Bytes: " << *opt->bytes << '\n';
    if (opt->seed)     std::cout << "Seed: "     << *opt->seed     << '\n';
    if (opt->out_file) std::cout << "Output File: " << *opt->out_file << '\n';
    return 0;
}