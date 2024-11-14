#include "abstract_plugin.h"
#include <clipper2/clipper.h>

namespace gddi {

inline float area_cover_rate(const std::vector<std::vector<int>> &polygon1,
                             const std::vector<std::vector<int>> &polygon2) {
    // 创建 Clipper 对象
    Clipper2Lib::Clipper64 clipper;

    Clipper2Lib::Paths64 polygon1_paths;
    Clipper2Lib::Paths64 polygon2_paths;

    Clipper2Lib::Path64 path1;
    for (const auto &point : polygon1) { path1.push_back(Clipper2Lib::Point64(point[0], point[1])); }
    polygon1_paths.push_back(path1);

    Clipper2Lib::Path64 path2;
    for (const auto &point : polygon2) { path2.push_back(Clipper2Lib::Point64(point[0], point[1])); }
    polygon2_paths.push_back(path2);

    // 添加多边形到 Clipper 对象
    clipper.AddSubject(polygon1_paths);
    clipper.AddClip(polygon2_paths);

    // 计算交集
    Clipper2Lib::Paths64 intersection;
    clipper.Execute(Clipper2Lib::ClipType::Intersection, Clipper2Lib::FillRule::NonZero, intersection);

    // 计算重叠面积占比
    return Clipper2Lib::Area(intersection)
        / std::min(Clipper2Lib::Area(polygon1_paths), Clipper2Lib::Area(polygon2_paths));
}

class RoiFilter : public AbstractPlugin {
public:
    RoiFilter(const std::string &name) : AbstractPlugin(name) {
        this->bind_property("region_of_interest", region_of_interest_, "感兴趣区域");
        this->bind_property("iou_threshold", iou_threshold_, "重叠面积阈值");
    }

    std::vector<AlgoObject> infer_result_process(const std::vector<AlgoObject> &objects) override {
        std::vector<AlgoObject> new_objects;

        for (const auto &[key, roi_points] : region_of_interest_) {
            // 目标框与ROI区域重叠面积需大于阈值
            for (auto &item : objects) {
                std::vector<std::vector<int>> box_points;
                box_points.emplace_back(std::vector<int>{item.rect[0], item.rect[1]});
                box_points.emplace_back(std::vector<int>{item.rect[0] + item.rect[2], item.rect[1]});
                box_points.emplace_back(std::vector<int>{item.rect[0] + item.rect[2], item.rect[1] + item.rect[3]});
                box_points.emplace_back(std::vector<int>{item.rect[0], item.rect[1] + item.rect[3]});

                // 目标框重叠面积大于阈值并且关键点全部在ROI区域内
                if (area_cover_rate(box_points, roi_points) >= iou_threshold_) { new_objects.emplace_back(item); }
            }
        }

        return new_objects;
    }

    std::vector<AlgoObject> tracked_result_process(const std::vector<AlgoObject> &objects) override {
        std::vector<AlgoObject> new_objects;

        for (const auto &[key, roi_points] : region_of_interest_) {
            // 目标框与ROI区域重叠面积需大于阈值
            for (auto &item : objects) {
                std::vector<std::vector<int>> box_points;
                box_points.emplace_back(std::vector<int>{item.rect[0], item.rect[1]});
                box_points.emplace_back(std::vector<int>{item.rect[0] + item.rect[2], item.rect[1]});
                box_points.emplace_back(std::vector<int>{item.rect[0] + item.rect[2], item.rect[1] + item.rect[3]});
                box_points.emplace_back(std::vector<int>{item.rect[0], item.rect[1] + item.rect[3]});

                // 目标框重叠面积大于阈值并且关键点全部在ROI区域内
                if (area_cover_rate(box_points, roi_points) >= iou_threshold_) { new_objects.emplace_back(item); }
            }
        }

        return new_objects;
    }

private:
    float iou_threshold_{0.5f};
    std::map<std::string, std::vector<std::vector<int>>> region_of_interest_;
};

}// namespace gddi

// 导出插件创建函数
extern "C" AbstractPlugin *create() { return new gddi::RoiFilter("区域过滤"); }

// 导出插件销毁函数
extern "C" void destroy(AbstractPlugin *plugin) { delete plugin; }