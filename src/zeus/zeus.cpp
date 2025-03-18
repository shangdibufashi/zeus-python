#include "main.hpp"
#include <map>
#include <opencv2/opencv.hpp>
// #include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <string>

namespace py = pybind11;
std::string wrapper_read_file_as_str(std::string srcfile){
    return read_file_as_str(srcfile);
}

std::string test_str(std::string &info)
{
    info="updated in cpp code";
    return "hello";
}
std::string test_dict(std::map<std::string, std::string> &dict)
{
    std::string result = "";
    for (const auto &pair : dict) {
        result += pair.first + ": " + pair.second + "\n";
    }
    dict["added"] = "value";
    return result;
}


py::array_t<uint8_t> wrapper_cv2_img_concat(py::array_t<uint8_t> &img1_np, py::array_t<uint8_t> &img2_np)
{
    py::buffer_info info1 = img1_np.request();
    py::buffer_info info2 = img2_np.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_8UC3, (uint8_t *) info1.ptr);
    cv::Mat img2(static_cast<int>(info2.shape[0]), static_cast<int>(info2.shape[1]),
                 CV_8UC3, (uint8_t *) info2.ptr);

    cv::Mat result = cv2_img_concat(img1, img2);

    // py::array_t<uint8_t> result_np({result.rows, result.cols, result.channels()}, result.data);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint8_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint8_t) * clone->channels() * clone->cols, sizeof(uint8_t) * clone->channels(), sizeof(uint8_t)}, // strides
        reinterpret_cast<uint8_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

py::array_t<uint16_t> wrapper_cv2_img_concat_to_bit16(py::array_t<uint8_t> &img1_np, py::array_t<uint8_t> &img2_np)
{
    py::buffer_info info1 = img1_np.request();
    py::buffer_info info2 = img2_np.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_8UC3, (uint8_t *) info1.ptr);
    cv::Mat img2(static_cast<int>(info2.shape[0]), static_cast<int>(info2.shape[1]),
                 CV_8UC3, (uint8_t *) info2.ptr);

    cv::Mat result = cv2_img_concat_to_bit16(img1, img2);

    // py::array_t<uint8_t> result_np({result.rows, result.cols, result.channels()}, result.data);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint16_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint16_t) * clone->channels() * clone->cols, sizeof(uint16_t) * clone->channels(), sizeof(uint16_t)}, // strides
        reinterpret_cast<uint16_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

/**
 * 3D LUT interpolation 8 * 8 - 64 * 64
 * @param src
 * @param lutPath
 * @return Mat
 */
// cv::Mat apply3DLut(const cv::Mat &src, const std::string &lutPath, InterpMode mode);
py::array_t<uint8_t> wrapper_apply_3dlut(py::array_t<uint8_t> img1_np_rgb, py::array_t<uint8_t> img2_np_rgb, const std::string mode)
{

    py::buffer_info info1 = img1_np_rgb.request();
    py::buffer_info info2 = img2_np_rgb.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_8UC3, (uint8_t *) info1.ptr);
    cv::Mat imgLut(static_cast<int>(info2.shape[0]), static_cast<int>(info2.shape[1]),
                 CV_8UC3, (uint8_t *) info2.ptr);

    cv::Mat result = apply3DLut(img1, imgLut, mode);

    // py::array_t<uint8_t> result_np({result.rows, result.cols, result.channels()}, result.data);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint8_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint8_t) * clone->channels() * clone->cols, sizeof(uint8_t) * clone->channels(), sizeof(uint8_t)}, // strides
        reinterpret_cast<uint8_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

py::array_t<uint16_t> wrapper_apply_3dlut_16bit(py::array_t<uint16_t> img1_np_rgb, py::array_t<uint16_t> img2_np_rgb, const std::string mode)
{

    py::buffer_info info1 = img1_np_rgb.request();
    py::buffer_info info2 = img2_np_rgb.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_16UC3, (uint16_t *) info1.ptr);
    cv::Mat imgLut(static_cast<int>(info2.shape[0]), static_cast<int>(info2.shape[1]),
                 CV_16UC3, (uint16_t *) info2.ptr);

    cv::Mat result = apply3DLut(img1, imgLut, mode);

    // py::array_t<uint16_t> result_np({result.rows, result.cols, result.channels()}, reinterpret_cast<uint16_t*>(result.data));

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint16_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint16_t) * clone->channels() * clone->cols, sizeof(uint16_t) * clone->channels(), sizeof(uint16_t)}, // strides
        reinterpret_cast<uint16_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

py::array_t<uint8_t> wrapper_apply_3dlut_file(py::array_t<uint8_t> img1_np_rgb, const std::string lutPath, const std::string mode)
{

    py::buffer_info info1 = img1_np_rgb.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_8UC3, (uint8_t *) info1.ptr);

    cv::Mat result = apply3DLut(img1, lutPath, mode);

    // py::array_t<uint8_t> result_np({result.rows, result.cols, result.channels()}, result.data);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint8_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint8_t) * clone->channels() * clone->cols, sizeof(uint8_t) * clone->channels(), sizeof(uint8_t)}, // strides
        reinterpret_cast<uint8_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}


py::array_t<uint16_t> wrapper_apply_3dlut_16bit_file(py::array_t<uint16_t> img1_np_rgb, const std::string lutPath, const std::string mode)
{

    py::buffer_info info1 = img1_np_rgb.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_16UC3, (uint16_t *) info1.ptr);

    cv::Mat result = apply3DLut(img1, lutPath, mode);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint16_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint16_t) * clone->channels() * clone->cols, sizeof(uint16_t) * clone->channels(), sizeof(uint16_t)}, // strides
        reinterpret_cast<uint16_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

/**
 * 转档LUT专用 Trilinear interpolation插值方法, b - g - r order
 * @param src
 * @param lutPath
 * @param wb {r g b}
 * @return
 */
// cv::Mat apply3DLutNew(const cv::Mat &src, const std::string &lutPath);
py::array_t<uint8_t> wrapper_apply_3d_lut_post_file(py::array_t<uint8_t> img1_np_rgb, const std::string lutPath)
{
    py::buffer_info info1 = img1_np_rgb.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_8UC3, (uint8_t *) info1.ptr);

    cv::Mat result = apply3DLutFile(img1, lutPath);

    // py::array_t<uint8_t> result_np({result.rows, result.cols, result.channels()}, result.data);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint8_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint8_t) * clone->channels() * clone->cols, sizeof(uint8_t) * clone->channels(), sizeof(uint8_t)}, // strides
        reinterpret_cast<uint8_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

/**
 * 转档LUT专用 Trilinear interpolation插值方法, b - g - r order
 * @param src
 * @param lutPath
 * @param wb {r g b}
 * @return
 */
// cv::Mat apply3DLutNew(const cv::Mat &src, const std::string &lutPath);
py::array_t<uint16_t> wrapper_apply_3d_lut_16bit_post_file(py::array_t<uint16_t> img1_np_rgb, const std::string lutPath)
{
    py::buffer_info info1 = img1_np_rgb.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_8UC3, (uint16_t *) info1.ptr);

    cv::Mat result = apply3DLutFile(img1, lutPath);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint16_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint16_t) * clone->channels() * clone->cols, sizeof(uint16_t) * clone->channels(), sizeof(uint16_t)}, // strides
        reinterpret_cast<uint16_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

py::array_t<uint8_t> wrapper_apply_3d_lut_post(py::array_t<uint8_t> img1_np_rgb, py::array_t<uint8_t> lut_np_rgb)
{
    py::buffer_info info1 = img1_np_rgb.request();
    py::buffer_info info2 = lut_np_rgb.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_8UC3, (uint8_t *) info1.ptr);
    cv::Mat lut(static_cast<int>(info2.shape[0]), static_cast<int>(info2.shape[1]),
                 CV_8UC3, (uint8_t *) info2.ptr);

    cv::Mat result = apply3DLutNew(img1, lut);

    // py::array_t<uint8_t> result_np({result.rows, result.cols, result.channels()}, result.data);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint8_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint8_t) * clone->channels() * clone->cols, sizeof(uint8_t) * clone->channels(), sizeof(uint8_t)}, // strides
        reinterpret_cast<uint8_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

py::array_t<uint16_t> wrapper_apply_3d_lut_post_16bit(py::array_t<uint16_t> img1_np_rgb, py::array_t<uint16_t> lut_np_rgb)
{
    py::buffer_info info1 = img1_np_rgb.request();
    py::buffer_info info2 = lut_np_rgb.request();

    // Notice: img1 and lut should be CV_16UC3
    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_16UC3, (uint16_t *) info1.ptr);
    cv::Mat lut(static_cast<int>(info2.shape[0]), static_cast<int>(info2.shape[1]),
                 CV_16UC3, (uint16_t *) info2.ptr);

    if (img1.depth() != CV_16U) {
        throw std::runtime_error("Input image depth is not 16bit");
    }
    if (lut.depth() != CV_16U) {
        throw std::runtime_error("Input lut depth is not 16bit");
    }
    cv::Mat result = apply3DLutNew(img1, lut);
    if (result.depth() != CV_16U) {
        throw std::runtime_error("Input result depth is not 16bit");
    }

    // py::array_t<uint16_t> result_np({result.rows, result.cols, result.channels()}, reinterpret_cast<uint16_t*>(result.data));

    // return result_np;

    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint16_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint16_t) * clone->channels() * clone->cols, sizeof(uint16_t) * clone->channels(), sizeof(uint16_t)}, // strides
        reinterpret_cast<uint16_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}



py::array_t<uint16_t> wrapper_vibrance_16bit(py::array_t<uint16_t> img1_np_rgb, int adjustment)
{
    py::buffer_info info1 = img1_np_rgb.request();

    // Notice: img1 and lut should be CV_16UC3
    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_16UC3, (uint16_t *) info1.ptr);

    if (img1.depth() != CV_16U) {
        throw std::runtime_error("Input image depth is not 16bit");
    }

    cv::Mat result = img1.clone();
    vibrance_16bit(result, adjustment);
    if (result.depth() != CV_16U) {
        throw std::runtime_error("Input result depth is not 16bit");
    }

    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint16_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint16_t) * clone->channels() * clone->cols, sizeof(uint16_t) * clone->channels(), sizeof(uint16_t)}, // strides
        reinterpret_cast<uint16_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

py::array_t<uint8_t> wrapper_vibrance_8bit(py::array_t<uint8_t> img1_np_rgb, int adjustment)
{
    py::buffer_info info1 = img1_np_rgb.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_8UC3, (uint8_t *) info1.ptr);

    cv::Mat result = img1.clone();
    vibrance_8bit(result, adjustment);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint8_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint8_t) * clone->channels() * clone->cols, sizeof(uint8_t) * clone->channels(), sizeof(uint8_t)}, // strides
        reinterpret_cast<uint8_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

py::array_t<uint8_t> wrapper_skinWhiteMat8(py::array_t<uint8_t> img1_np_rgb, float beta)
{
    py::buffer_info info1 = img1_np_rgb.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_8UC3, (uint8_t *) info1.ptr);

    cv::Mat result = skinWhiteMat8(img1, beta);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint8_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint8_t) * clone->channels() * clone->cols, sizeof(uint8_t) * clone->channels(), sizeof(uint8_t)}, // strides
        reinterpret_cast<uint8_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

py::array_t<uint8_t> wrapper_autoBrightnessAndContrast(py::array_t<uint8_t> img1_np_rgb)
{
    py::buffer_info info1 = img1_np_rgb.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_8UC3, (uint8_t *) info1.ptr);

    cv::Mat result = autoBrightnessAndContrast(img1);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint8_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint8_t) * clone->channels() * clone->cols, sizeof(uint8_t) * clone->channels(), sizeof(uint8_t)}, // strides
        reinterpret_cast<uint8_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}


py::array_t<uint8_t> wrapper_apply3DLut8BitAllInOne(py::array_t<uint8_t> img1_np_rgb, py::array_t<uint8_t> lut_np_rgb, float skin_white, int vibrance)
{
    py::buffer_info info1 = img1_np_rgb.request();
    py::buffer_info info2 = lut_np_rgb.request();

    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_8UC3, (uint8_t *) info1.ptr);
    cv::Mat lut(static_cast<int>(info2.shape[0]), static_cast<int>(info2.shape[1]),
                 CV_8UC3, (uint8_t *) info2.ptr);

    cv::Mat result = apply3DLut8BitAllInOne(img1, lut, skin_white, vibrance);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint8_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint8_t) * clone->channels() * clone->cols, sizeof(uint8_t) * clone->channels(), sizeof(uint8_t)}, // strides
        reinterpret_cast<uint8_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

py::array_t<uint8_t> wrapper_apply3DLut16BitAllInOne(py::array_t<uint16_t> img1_np_rgb, py::array_t<uint16_t> lut_np_rgb, float skin_white, int vibrance)
{
    py::buffer_info info1 = img1_np_rgb.request();
    py::buffer_info info2 = lut_np_rgb.request();

    // Notice: img1 and lut should be CV_16UC3
    cv::Mat img1(static_cast<int>(info1.shape[0]), static_cast<int>(info1.shape[1]),
                 CV_16UC3, (uint16_t *) info1.ptr);
    cv::Mat lut(static_cast<int>(info2.shape[0]), static_cast<int>(info2.shape[1]),
                 CV_16UC3, (uint16_t *) info2.ptr);

    if (img1.depth() != CV_16U) {
        throw std::runtime_error("Input image depth is not 16bit");
    }
    if (lut.depth() != CV_16U) {
        throw std::runtime_error("Input lut depth is not 16bit");
    }
    cv::Mat result = apply3DLut16BitAllInOne(img1, lut, skin_white, vibrance); // 返回的是<8bit>
    if (result.depth() != CV_8U) {
        throw std::runtime_error("Input result depth is not 16bit");
    }

    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint8_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint8_t) * clone->channels() * clone->cols, sizeof(uint8_t) * clone->channels(), sizeof(uint8_t)}, // strides
        reinterpret_cast<uint8_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

PYBIND11_MODULE(zeus, m)
{
    py::module_::import("numpy");
    m.def("read_file_as_str", &wrapper_read_file_as_str, "Where there is a will, there is a way");
    m.def("test_str", &test_str, "Where there is a will, there is a way");
    m.def("test_dict", &test_dict, "Where there is a will, there is a way");
    m.def("cv2_img_concat", &wrapper_cv2_img_concat, "Where there is a will, there is a way");
    m.def("cv2_img_concat_to_bit16", &wrapper_cv2_img_concat_to_bit16, "Where there is a will, there is a way");
    m.def("apply_3dlut", &wrapper_apply_3dlut, "Where there is a will, there is a way");
    m.def("apply_3dlut_16bit", &wrapper_apply_3dlut_16bit, "Where there is a will, there is a way");
    m.def("apply_3dlut_file", &wrapper_apply_3dlut_file, "Where there is a will, there is a way");
    m.def("apply_3dlut_16bit_file", &wrapper_apply_3dlut_16bit_file, "Where there is a will, there is a way");
    m.def("apply_3d_lut_post_file", &wrapper_apply_3d_lut_post_file, "Where there is a will, there is a way");
    m.def("apply_3d_lut_16bit_post_file", &wrapper_apply_3d_lut_16bit_post_file, "Where there is a will, there is a way");
    m.def("apply_3d_lut_post", &wrapper_apply_3d_lut_post, "Where there is a will, there is a way");
    m.def("apply_3d_lut_post_16bit", &wrapper_apply_3d_lut_post_16bit, "Where there is a will, there is a way");

    m.def("apply3DLut16BitAllInOne", &wrapper_apply3DLut16BitAllInOne, "Where there is a will, there is a way");
    m.def("apply3DLut8BitAllInOne", &wrapper_apply3DLut8BitAllInOne, "Where there is a will, there is a way");
    m.def("skinWhiteMat8", &wrapper_skinWhiteMat8, "Where there is a will, there is a way");
    m.def("autoBrightnessAndContrast", &wrapper_autoBrightnessAndContrast, "Where there is a will, there is a way");
    m.def("vibrance_8bit", &wrapper_vibrance_8bit, "Where there is a will, there is a way");
    m.def("vibrance_16bit", &wrapper_vibrance_16bit, "Where there is a will, there is a way");
}
