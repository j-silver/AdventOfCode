//
// Created by giuseppe on 04/12/2020.
//
#include <fstream>
#include <regex>
#include <filesystem>
#include <iostream>
#include <charconv>
#include <set>

const std::regex fields {R"(([a-z]+):([#a-z0-9]+))"};
const std::regex color {"#[0-9a-f]{6}"};
const std::set<std::string> EyeColors {
        "amb", "blu", "brn", "gry", "grn", "hzl", "oth"
};
const std::regex PassNumber {"[0-9]{9}"};

class Passport {
public:
    explicit Passport(std::string const& data)
    {
        for (std::sregex_iterator match {data.cbegin(), data.cend(), fields};
             match != std::sregex_iterator{};
             ++match)
        {
                std::string field {(*match)[1]};
                if (field == "byr") { _byr = (*match)[2]; }
                if (field == "iyr") { _iyr = (*match)[2]; }
                if (field == "eyr") { _eyr = (*match)[2]; }
                if (field == "hgt") { _hgt = (*match)[2]; }
                if (field == "hcl") { _hcl = (*match)[2]; }
                if (field == "ecl") { _ecl = (*match)[2]; }
                if (field == "pid") { _pid = (*match)[2]; }
                if (field == "cid") { _cid = (*match)[2]; }
        }
        _valid = check_byr() && check_iyr() && check_eyr() && check_hgt() && check_hcl() && check_ecl() && check_pid();
    }

    bool check_eyr() const
    {
        if (_eyr.length() != 4 || std::stoi(_eyr) < 2020 || std::stoi(_eyr) > 2030) {
            return false;
        }
        return true;
    }

    bool check_iyr() const
    {
        if (_iyr.length() != 4 || std::stoi(_iyr) < 2010 || std::stoi(_iyr) > 2020) {
            return false;
        }
        return true;
    }

    bool check_byr() const
    {
        if (_byr.length() != 4 || std::stoi(_byr) < 1920 || std::stoi(_byr) > 2002) {
            return false;
        }
        return true;
    }

    bool check_hgt() const
    {
        int height {0};
        auto const result {std::from_chars(&_hgt[0], &_hgt[3], height)};
        if (result.ec == std::errc::invalid_argument) {
            return false;
        }
        if (_hgt.ends_with("cm")) {
            if (_hgt.length() != 5) {
                return false;
            }
            if (height < 150 || height > 193) {
                return false;
            }
        }
        else if (_hgt.ends_with("in")) {
            if (_hgt.length() != 4) {
                return false;
            }
            if (height < 59 || height > 76) {
                return false;
            }
        }
        else {
            return false;
        }

        return true;
    }

    bool check_hcl() const
    {
        return _hcl.length() == 7 && std::regex_match(_hcl, color);
    }

    bool check_ecl() const
    {
        return std::any_of(EyeColors.cbegin(), EyeColors.cend(), [this](auto const& c){ return c == _ecl;} );
    }

    bool check_pid() const
    {
        return std::regex_match(_pid, PassNumber);
    }

    [[nodiscard]] bool is_valid() const
    {
        return _valid;
    }

private:
    std::string _byr {};
    std::string _iyr {};
    std::string _eyr {};
    std::string _hgt {};
    std::string _hcl {};
    std::string _ecl {};
    std::string _pid {};
    std::string _cid {};
    bool _valid {false};
};

std::vector<Passport> read_file(std::ifstream& fileStream)
{
    std::vector<Passport> passports;
    for ( ; fileStream.good(); ) {
        std::string pass;
        for (std::string line; std::getline(fileStream, line);) {
            if (!line.empty()) {
                pass += line + " ";
            }
            else {
                break;
            }
        }
        passports.emplace_back(pass);
    }

    return passports;
}

int main()
{
    std::ifstream file {std::filesystem::path {"../../files/input4.txt"}};
    auto const passports {read_file(file)};
    auto const numberOfValidPassports {
        std::count_if(passports.cbegin(), passports.cend(), [](auto const& p){ return p.is_valid(); })
    };
    std::cout << numberOfValidPassports << '\n';
}