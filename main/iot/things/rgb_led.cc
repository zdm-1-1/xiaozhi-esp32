
#include "iot/thing.h"
#include "boards/bread-compact-wifi/config.h"

#include <driver/gpio.h>
#include <esp_err.h>
#include <string>

#ifdef CONFIG_BOARD_TYPE_BREAD_COMPACT_WIFI

namespace iot {

class RgbLed : public Thing {
private:
    std::string color_ = "off";

    void SetRgb(int red, int green, int blue) {
        gpio_set_level(RGB_RED_GPIO, red);
        gpio_set_level(RGB_GREEN_GPIO, green);
        gpio_set_level(RGB_BLUE_GPIO, blue);
    }

    void SetColor(const std::string& color) {
        if (color == "red" || color == "红色") {
            SetRgb(1, 0, 0);
        } else if (color == "green" || color == "绿色") {
            SetRgb(0, 1, 0);
        } else if (color == "blue" || color == "蓝色") {
            SetRgb(0, 0, 1);
        } else if (color == "yellow" || color == "黄色") {
            SetRgb(1, 1, 0);
        } else if (color == "purple" || color == "紫色") {
            SetRgb(1, 0, 1);
        } else if (color == "cyan" || color == "青色") {
            SetRgb(0, 1, 1);
        } else if (color == "white" || color == "白色") {
            SetRgb(1, 1, 1);
        } else if (color == "off" || color == "关闭") {
            SetRgb(0, 0, 0);
        } else {
            return;
        }
        color_ = color;
    }

public:
    RgbLed() : Thing("RgbLed", "外接RGB三色彩灯") {
        gpio_config_t config = {};
        config.pin_bit_mask =
            (1ULL << RGB_RED_GPIO) |
            (1ULL << RGB_GREEN_GPIO) |
            (1ULL << RGB_BLUE_GPIO);
        config.mode = GPIO_MODE_OUTPUT;

        ESP_ERROR_CHECK(gpio_config(&config));
        SetColor("off");

        properties_.AddStringProperty(
            "color", "当前颜色",
            [this]() { return color_; }
        );

        ParameterList params;
        params.AddParameter(Parameter(
            "color",
            "颜色：红色、绿色、蓝色、黄色、紫色、青色、白色、关闭",
            kValueTypeString
        ));

        methods_.AddMethod(
            "SetColor", "设置外接RGB彩灯颜色",
            params,
            [this](const ParameterList& p) {
                SetColor(p["color"].string());
            }
        );
    }
};

} // namespace iot

DECLARE_THING(RgbLed);

#endif
