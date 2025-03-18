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
std::string wrapper_get_3dlut_type(std::string srcfile){
    return get_3dlut_type(srcfile);
}
std::string wrapper_get_cam_wb(std::string srcfile){
    return get_cam_wb(srcfile);
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
// std::string raw_imread(std::map<std::string, std::string> mParams);
std::string imread(std::map<std::string, std::string> dict)
{
    return raw_imread(dict);
}

py::array_t<uint16_t> wrapper_raw2tiff(std::map<std::string, std::string> dict)
{
    cv::Mat result = raw2tiff(dict); // image in 16bit

    // py::array_t<uint16_t> result_np({result.rows, result.cols, result.channels()}, result.data);
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

py::array_t<uint16_t> wrapper_raw2tiffext(std::map<std::string, std::string> dict,
                                         py::array_t<uint8_t> &thumbBodyMaskGrayNP,
                                         py::array_t<uint8_t> &thumbSkinMaskGrayNP,
                                         py::array_t<uint8_t> &thumbRGBNP)
{

    py::buffer_info thumbBodyMaskGrayInfo = thumbBodyMaskGrayNP.request();
    py::buffer_info thumbSkinMaskGrayInfo = thumbSkinMaskGrayNP.request();
    py::buffer_info thumbRGBInfo = thumbRGBNP.request();

    cv::Mat thumbBodyMaskGray(static_cast<int>(thumbBodyMaskGrayInfo.shape[0]), static_cast<int>(thumbBodyMaskGrayInfo.shape[1]),
                              CV_8UC1, (uint8_t *) thumbBodyMaskGrayInfo.ptr);
    cv::Mat thumbSkinMaskGray(static_cast<int>(thumbSkinMaskGrayInfo.shape[0]), static_cast<int>(thumbSkinMaskGrayInfo.shape[1]),
                              CV_8UC1, (uint8_t *) thumbSkinMaskGrayInfo.ptr);
    cv::Mat thumbRGB(static_cast<int>(thumbRGBInfo.shape[0]), static_cast<int>(thumbRGBInfo.shape[1]),
                     CV_8UC3, (uint8_t *) thumbRGBInfo.ptr);

    cv::Mat result = raw2tiffext(dict, thumbBodyMaskGray, thumbSkinMaskGray, thumbRGB);

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
std::string wrapper_createProcParams(std::map<std::string, std::string> dict,
                                         py::array_t<uint8_t> &thumbBodyMaskGrayNP,
                                         py::array_t<uint8_t> &thumbSkinMaskGrayNP,
                                         py::array_t<uint8_t> &thumbRGBNP)
{

    py::buffer_info thumbBodyMaskGrayInfo = thumbBodyMaskGrayNP.request();
    py::buffer_info thumbSkinMaskGrayInfo = thumbSkinMaskGrayNP.request();
    py::buffer_info thumbRGBInfo = thumbRGBNP.request();

    cv::Mat thumbBodyMaskGray(static_cast<int>(thumbBodyMaskGrayInfo.shape[0]), static_cast<int>(thumbBodyMaskGrayInfo.shape[1]),
                              CV_8UC1, (uint8_t *) thumbBodyMaskGrayInfo.ptr);
    cv::Mat thumbSkinMaskGray(static_cast<int>(thumbSkinMaskGrayInfo.shape[0]), static_cast<int>(thumbSkinMaskGrayInfo.shape[1]),
                              CV_8UC1, (uint8_t *) thumbSkinMaskGrayInfo.ptr);
    cv::Mat thumbRGB(static_cast<int>(thumbRGBInfo.shape[0]), static_cast<int>(thumbRGBInfo.shape[1]),
                     CV_8UC3, (uint8_t *) thumbRGBInfo.ptr);

    return createProcParams(dict, thumbBodyMaskGray, thumbSkinMaskGray, thumbRGB);

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

py::array_t<uint8_t> wrapper_genThumb(const std::string &input,
                                      const std::string &pp3Path,
                                      const std::string &bundlePath,
                                      bool isSmallNEF)
{
    cv::Mat result = genThumb(input, pp3Path, bundlePath, isSmallNEF,8);

    // std::cout << "wrapper_genThumb::capsule::result" << std::endl;
    // py::array_t<uint8_t> result_np({result.rows, result.cols, result.channels()}, result.data);

    // return result_np;
    cv::Mat* clone = new cv::Mat(result.clone());
    result.release();
    // std::cout << "wrapper_genThumb::capsule::clone" << std::endl;

    // Create a capsule that deletes the cloned cv::Mat when it is done.
    py::capsule capsule(clone, [](void* ptr) {
        cv::Mat* matPtr = reinterpret_cast<cv::Mat*>(ptr);
        // std::cout << "wrapper_genThumb::capsule::delete" << std::endl;
        delete matPtr;
    });

    // Create the NumPy array with the correct data pointer, shape, and capsule for memory management.
    return py::array_t<uint8_t>(
        {clone->rows, clone->cols, clone->channels()}, // shape
        {sizeof(uint8_t) * clone->channels() * clone->cols, sizeof(uint8_t) * clone->channels(), sizeof(uint8_t)}, // strides
        reinterpret_cast<uint8_t*>(clone->data), // data pointer
        capsule); // memory management capsule
}

py::array_t<uint16_t> wrapper_genThumbTiff(const std::string &input,
                                      const std::string &pp3Path,
                                      const std::string &bundlePath,
                                      bool isSmallNEF)
{
    cv::Mat result = genThumb(input, pp3Path, bundlePath, isSmallNEF, 16);

    // py::array_t<uint16_t> result_np({result.rows, result.cols, result.channels()}, reinterpret_cast<uint16_t*>(result.data));

    // return result_np;

    


    // Clone the result to ensure we have a continuous block of memory.
    /**
     The decision to clone the cv::Mat object before using it with a py::capsule for memory management in Python, instead of using the original cv::Mat object directly, depends on the specifics of your application and how the cv::Mat object is managed and used. Here are the key considerations:

        Continuity of Data
        Continuity: cv::Mat::clone() ensures that the resulting cv::Mat object has its own copy of the data in a continuous block of memory. This is particularly important if the original cv::Mat object was obtained through operations that might result in non-continuous memory layouts (e.g., cropping). Non-continuous data can lead to issues when interfacing with other libraries or APIs expecting a continuous memory layout, such as when creating a NumPy array that directly uses this data.

        Direct Usage: If the original cv::Mat object is already guaranteed to have a continuous memory layout (e.g., it was just read from a file or created from scratch), and you are not concerned about modifying the original data, you might directly use it without cloning. However, you must ensure that the cv::Mat's data will not be deallocated or go out of scope while still in use by Python.

        Ownership and Modifications
        Ownership: Cloning with cv::Mat::clone() creates a new cv::Mat object with its own copy of the data, effectively transferring ownership of this copy to the Python side (via the capsule). This means changes made to the NumPy array will not affect the original cv::Mat object, and vice versa. This is often desirable to prevent unexpected side effects across different parts of a program.

        Direct Usage: Using the original cv::Mat directly is more memory efficient since it avoids the need to create a copy of the data. This approach can be suitable when memory usage is a concern, and you are sure that the lifetime of the cv::Mat object and its data is properly managed. However, this approach requires careful management to ensure that the cv::Mat object does not get modified or released while the Python side still needs access to its data.

        Performance Considerations
        Cloning: Cloning a cv::Mat object incurs additional memory and performance overhead due to the need to allocate memory for and copy the data. This might be a concern in performance-critical applications or when working with very large images.

        Direct Usage: Using the cv::Mat object directly is more efficient in terms of performance and memory usage, as long as the issues mentioned above are properly addressed.

        Conclusion
        The choice between cloning the cv::Mat object and using it directly when creating a py::capsule depends on the requirements of your application regarding data continuity, memory and performance considerations, and whether the data needs to be isolated between C++ and Python. Cloning provides a safer, isolated approach at the cost of additional memory and computational overhead, while direct usage is more efficient but requires careful management of the object's lifetime and modifications.
    */
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
    m.def("createProcParams", &wrapper_createProcParams, "Where there is a will, there is a way");
    m.def("read_file_as_str", &wrapper_read_file_as_str, "Where there is a will, there is a way");
    m.def("get_3dlut_type", &wrapper_get_3dlut_type, "Where there is a will, there is a way");
    m.def("get_cam_wb", &wrapper_get_cam_wb, "Where there is a will, there is a way");
    m.def("imread", &imread, "Where there is a will, there is a way");
    m.def("raw2tiff", &wrapper_raw2tiff, "Where there is a will, there is a way");
    m.def("raw2tiffext", &wrapper_raw2tiffext, "Where there is a will, there is a way");
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
    m.def("genThumbToFile", &genThumbToFile, "Where there is a will, there is a way");
    m.def("genThumb", &wrapper_genThumb, "Where there is a will, there is a way");
    m.def("genThumbTiff", &wrapper_genThumbTiff, "Where there is a will, there is a way");

    m.def("apply3DLut16BitAllInOne", &wrapper_apply3DLut16BitAllInOne, "Where there is a will, there is a way");
    m.def("apply3DLut8BitAllInOne", &wrapper_apply3DLut8BitAllInOne, "Where there is a will, there is a way");
    m.def("skinWhiteMat8", &wrapper_skinWhiteMat8, "Where there is a will, there is a way");
    m.def("autoBrightnessAndContrast", &wrapper_autoBrightnessAndContrast, "Where there is a will, there is a way");
    m.def("vibrance_8bit", &wrapper_vibrance_8bit, "Where there is a will, there is a way");
    m.def("vibrance_16bit", &wrapper_vibrance_16bit, "Where there is a will, there is a way");
}
