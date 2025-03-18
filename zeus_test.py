import pyairaw.pyairaw as pyairaw
import cv2
import numpy as np
import logging
import os
import json
import time
import argparse

# https://github.com/Algomorph/pyboostcvconverter/tree/master
logging.basicConfig(
    level=logging.INFO,
    format = "%(asctime)s %(levelname)-1.1s L%(lineno)-3.3d %(process)-5.5d [%(name)-12.12s] %(message)s",
    handlers=[logging.StreamHandler()]
)

logger = logging.getLogger("pyairaw_test")
logger.setLevel(logging.DEBUG)
pwd = os.path.dirname(os.path.realpath(__file__))
bundlePath=os.path.dirname(pwd);
def timer_decorator(func):
    def wrapper(*args, **kwargs):
        start_time = time.time()
        result = func(*args, **kwargs)
        end_time = time.time()
        t = 1000*(end_time - start_time)
        too_much = '[NOTICE]' if t > 1000 else ''
        logger.debug(f"Fun[{func.__name__}]{too_much} took {t:.2f}ms")
        return result
    return wrapper

def assert_exists(f):
    logger.debug(f"assert_exists: {f}")
    assert os.path.exists(f), f"file not found: {f}"
    
@timer_decorator
def test_cv2_img_concat():
    imgfile = f'{pwd}/assets/portrait.jpg'
    dstfile= f'{pwd}/assets/portrait_concat_result.jpg'
    assert_exists(imgfile)
    img1 = cv2.imread(imgfile)
    img2 = cv2.flip(cv2.imread(imgfile), 1)

    logger.debug(f"test_cv2_img_concat img1: {img1.shape}")
    logger.debug(f"test_cv2_img_concat img2: {img2.shape}")

    result = pyairaw.cv2_img_concat(img1, img2)
    cv2.imwrite(dstfile, result)
    logger.debug(f"test_cv2_img_concat result: {result.shape}")
    logger.debug(f"test_cv2_img_concat save to: {dstfile}")


@timer_decorator
def test_pyairaw_dict():
    dict_data = {"name": "John", "age": "25", "city": "New York"}
    result = pyairaw.test_dict(dict_data)
    logger.debug(f"test_pyairaw_dict result: {result}")
    logger.debug(f"test_pyairaw_dict dict_data: {dict_data}")

@timer_decorator
def test_pyairaw_test_str():
    txt="hi"
    info = pyairaw.test_str(txt)
    logger.debug(f"test_pyairaw_dict txt: {txt}")
    logger.debug(f"test_pyairaw_dict info: {info}")


@timer_decorator
def test_apply_3dlut_file():
    """
    mode_linear,
    mode_cosine,
    mode_cubic,
    mode_tetrahedral
    """
    imgfile = f'{pwd}/assets/portrait.jpg'
    dstfile = f'{pwd}/assets/portrait_lut_file_mode_cubic.jpg'
    lutfile = f'{pwd}/assets/skin_red.png'
    assert_exists(imgfile)
    assert_exists(lutfile)

    img = cv2.imread(imgfile)
    rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    result = pyairaw.apply_3dlut_file(rgb, lutfile,"mode_cubic")
    cv2.imwrite(dstfile, cv2.cvtColor(result, cv2.COLOR_RGB2BGR))
    logger.debug(f"test_apply_3dlut_file result: {result.shape}")
    logger.debug(f"test_apply_3dlut_file save to: {dstfile}")

@timer_decorator
def test_apply_3dlut():
    """
    mode_linear,
    mode_cosine,
    mode_cubic,
    mode_tetrahedral
    """
    imgfile = f'{pwd}/assets/portrait.jpg'
    dstfile = f'{pwd}/assets/portrait_lut_mode_linear.jpg'
    lutfile = f'{pwd}/assets/skin_red.png'
    assert_exists(imgfile)
    assert_exists(lutfile)

    img = cv2.imread(imgfile)
    rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

    imglut = cv2.imread(lutfile)
    rgblut = cv2.cvtColor(imglut, cv2.COLOR_BGR2RGB)

    result = pyairaw.apply_3dlut(rgb, rgblut,"mode_linear")
    cv2.imwrite(dstfile, cv2.cvtColor(result, cv2.COLOR_RGB2BGR))
    logger.debug(f"test_apply_3dlut result: {result.shape}")
    logger.debug(f"test_apply_3dlut save to: {dstfile}")

@timer_decorator
def test_apply_3dlut_16bit():
    """
    mode_linear,
    mode_cosine,
    mode_cubic,
    mode_tetrahedral
    """
    imgfile = f'{pwd}/assets/portrait.jpg'
    dstfile = f'{pwd}/assets/portrait_lut_mode_linear.jpg'
    lutfile = f'{pwd}/assets/skin_red.png'
    assert_exists(imgfile)
    assert_exists(lutfile)

    img = cv2.imread(imgfile)
    rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    rgb = rgb.astype(np.uint16) * 255

    imglut = cv2.imread(lutfile)
    rgblut = cv2.cvtColor(imglut, cv2.COLOR_BGR2RGB)
    rgblut = rgblut.astype(np.uint16) * 255

    result = pyairaw.apply_3dlut_16bit(rgb, rgblut,"mode_linear")
    # cv2.imwrite(dstfile, cv2.cvtColor(result, cv2.COLOR_RGB2BGR))
    logger.debug(f"test_apply_3dlut_16bit result: {result.shape} {result.dtype}")
    assert result.dtype == np.uint16, "apply_3dlut_16bit result dtype is not uint16"
    # logger.debug(f"test_apply_3dlut_16bit save to: {dstfile}")
    
@timer_decorator
def test_apply_3d_lut_post_file():
    imgfile = f'{pwd}/assets/portrait.jpg'
    dstfile = f'{pwd}/assets/portrait_lut_file.jpg'
    lutfile = f'{pwd}/assets/skin_red.png'
    assert_exists(imgfile)
    assert_exists(lutfile)

    img = cv2.imread(imgfile)
    rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    result = pyairaw.apply_3d_lut_post_file(rgb, lutfile)
    cv2.imwrite(dstfile, cv2.cvtColor(result, cv2.COLOR_RGB2BGR))
    logger.debug(f"test_apply_3d_lut_post_file result: {result.shape}")
    logger.debug(f"test_apply_3d_lut_post_file save to: {dstfile}")

@timer_decorator
def test_apply_3d_lut_post():
    imgfile = f'{pwd}/assets/portrait.jpg'
    dstfile = f'{pwd}/assets/portrait_lut_post.jpg'
    lutfile = f'{pwd}/assets/skin_red.png'
    assert_exists(imgfile)
    assert_exists(lutfile)

    img = cv2.imread(imgfile)
    rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

    imglut = cv2.imread(lutfile)
    rgblut = cv2.cvtColor(imglut, cv2.COLOR_BGR2RGB)

    result = pyairaw.apply_3d_lut_post(rgb, rgblut)
    cv2.imwrite(dstfile, cv2.cvtColor(result, cv2.COLOR_RGB2BGR))
    logger.debug(f"test_apply_3d_lut_post result: {result.shape}")
    logger.debug(f"test_apply_3d_lut_post save to: {dstfile}")

@timer_decorator
def test_apply_3d_lut_post_16bit():
    imgfile = f'{pwd}/assets/portrait.jpg'
    dstfile = f'{pwd}/assets/portrait_lut_post.jpg'
    lutfile = f'{pwd}/assets/skin_red.png'
    assert_exists(imgfile)
    assert_exists(lutfile)

    img = cv2.imread(imgfile)
    rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    rgb = rgb.astype(np.uint16) * 255

    imglut = cv2.imread(lutfile)
    rgblut = cv2.cvtColor(imglut, cv2.COLOR_BGR2RGB)*255
    rgblut = rgblut.astype(np.uint16) * 255

    result = pyairaw.apply_3d_lut_post_16bit(rgb, rgblut)
    # cv2.imwrite(dstfile, cv2.cvtColor(result, cv2.COLOR_RGB2BGR))
    logger.debug(f"test_apply_3d_lut_post_16bit result: {result.shape}, {result.dtype}")
    assert result.dtype == np.uint16, "apply_3d_lut_post_16bit result dtype is not uint16"
    # logger.debug(f"test_apply_3d_lut_post_16bit save to: {dstfile}")
   

@timer_decorator
def main():
    
    logger.debug(f"------------> test_pyairaw_test_str")
    test_pyairaw_test_str()
    logger.debug(f"------------> test_apply_3d_lut_post")
    test_apply_3d_lut_post()
    logger.debug(f"------------> test_apply_3d_lut_post_16bit")
    test_apply_3d_lut_post_16bit()
    logger.debug(f"------------> test_apply_3d_lut_post_file")
    test_apply_3d_lut_post_file()
    logger.debug(f"------------> test_apply_3dlut")
    test_apply_3dlut()
    logger.debug(f"------------> test_apply_3dlut_16bit")
    test_apply_3dlut_16bit()
    logger.debug(f"------------> test_apply_3dlut_file")
    test_apply_3dlut_file()
    logger.debug(f"------------> test_cv2_img_concat")
    test_cv2_img_concat()
    logger.debug(f"------------> test_pyairaw_dict")
    test_pyairaw_dict()
    logger.debug(f"------------> done <------------")


if __name__ == '__main__':
    main()