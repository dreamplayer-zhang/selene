// This file is part of the `Selene` library.
// Copyright 2017-2018 Michael Hofmann (https://github.com/kmhofmann).
// Distributed under MIT license. See accompanying LICENSE file in the top-level directory.

#ifndef SELENE_IMG_DYNAMIC_UNTYPED_LAYOUT_HPP
#define SELENE_IMG_DYNAMIC_UNTYPED_LAYOUT_HPP

/// @file

#include <selene/img/common/PixelFormat.hpp>
#include <selene/img/common/Types.hpp>

namespace sln {

/** \brief The layout for a dynamically typed image, holding information about width, height, number of channels, the
 * number of bytes per channel, and the image's row stride in bytes.
 */
class UntypedLayout
{
public:
  UntypedLayout() noexcept
      : width{PixelLength{0}}
      , height{PixelLength{0}}
      , nr_channels{0}
      , nr_bytes_per_channel{0}
      , stride_bytes{Stride{0}}
  { }

  UntypedLayout(PixelLength width_,
                PixelLength height_,
                std::int16_t nr_channels_,
                std::int16_t nr_bytes_per_channel_) noexcept
      : width(width_)
      , height(height_)
      , nr_channels(nr_channels_)
      , nr_bytes_per_channel(nr_bytes_per_channel_)
      , stride_bytes(width_ * nr_channels_ * nr_bytes_per_channel_)
  { }

  UntypedLayout(PixelLength width_,
                PixelLength height_,
                std::int16_t nr_channels_,
                std::int16_t nr_bytes_per_channel_,
                Stride stride_bytes_) noexcept
      : width(width_)
      , height(height_)
      , nr_channels(nr_channels_)
      , nr_bytes_per_channel(nr_bytes_per_channel_)
      , stride_bytes(stride_bytes_)
  { }

  PixelLength width;  ///< The image width in pixels.
  PixelLength height;  ///< The image height in pixels.
  std::int16_t nr_channels;  ///< The number of image channels
  std::int16_t nr_bytes_per_channel;  ///< The number of bytes used for a channel value.
  Stride stride_bytes;  ///< The image row stride in bytes. The layout may include additional padding bytes.

  std::ptrdiff_t nr_bytes_per_pixel() const noexcept;
  std::ptrdiff_t row_bytes() const noexcept;
  std::ptrdiff_t total_bytes() const noexcept;
  bool is_packed() const noexcept;
};

bool operator==(const UntypedLayout& l, const UntypedLayout& r);

bool operator!=(const UntypedLayout& l, const UntypedLayout& r);

// -----------

class UntypedImageSemantics
{
public:
  UntypedImageSemantics() noexcept
      : pixel_format(PixelFormat::Unknown)
      , sample_format(SampleFormat::Unknown)
  { }

  UntypedImageSemantics(PixelFormat pixel_format_,
                        SampleFormat sample_format_)
      : pixel_format(pixel_format_)
      , sample_format(sample_format_)
  { }

  PixelFormat pixel_format;
  SampleFormat sample_format;
};

bool operator==(const UntypedImageSemantics& l, const UntypedImageSemantics& r);

bool operator!=(const UntypedImageSemantics& l, const UntypedImageSemantics& r);

// ----------
// Implementation:

/** \brief Returns the number of bytes per pixel.
 *
 * @return The number of bytes per pixel.
 */
inline std::ptrdiff_t UntypedLayout::nr_bytes_per_pixel() const noexcept
{
  return nr_channels * nr_bytes_per_channel;
}

/** \brief Returns the number of data bytes occupied by each image row.
 *
 *  The value returned is equal to `(width * nr_bytes_per_pixel())`.
 * It follows that `stride_bytes >= row_bytes()`, since `stride_bytes` may include additional padding bytes.
 *
 * @return Number of data bytes occupied by each image row.
 */
inline std::ptrdiff_t UntypedLayout::row_bytes() const noexcept
{
  return width * nr_bytes_per_pixel();
}

/** \brief Returns the total number of bytes occupied by the image data in memory.
 *
 * @return Number of bytes occupied by the image data in memory.
 */
inline std::ptrdiff_t UntypedLayout::total_bytes() const noexcept
{
  return stride_bytes * height;
}

/** \brief Returns whether image data is stored packed in memory using this layout.
 *
 * @return True, if the image data is stored packed using this layout; false otherwise.
 */
inline bool UntypedLayout::is_packed() const noexcept
{
  return stride_bytes == width * nr_channels * nr_bytes_per_channel;
}

/** \brief Equality comparison for two untyped layouts.
 *
 * @param l The left-hand side layout to compare.
 * @param r The right-hand side layout to compare.
 * @return True, if the two layouts are identical; false otherwise.
 */
inline bool operator==(const UntypedLayout& l, const UntypedLayout& r)
{
  return l.width == r.width && l.height == r.height
         && l.nr_channels == r.nr_channels && l.nr_bytes_per_channel == r.nr_bytes_per_channel
         && l.stride_bytes == r.stride_bytes;
}

/** \brief Inequality comparison for two untyped layouts.
 *
 * @param l The left-hand side layout to compare.
 * @param r The right-hand side layout to compare.
 * @return True, if the two layouts are not equal; false otherwise.
 */
inline bool operator!=(const UntypedLayout& l, const UntypedLayout& r)
{
  return !(l == r);
}

/** \brief Equality comparison for two image semantics structures.
 *
 * @param l The left-hand side semantics structure to compare.
 * @param r The right-hand side semantics structure to compare.
 * @return True, if the two semantics structures are identical; false otherwise.
 */
inline bool operator==(const UntypedImageSemantics& l, const UntypedImageSemantics& r)
{
  return l.pixel_format == r.pixel_format && l.sample_format == r.sample_format;
}

/** \brief Inequality comparison for two image semantics structures.
 *
 * @param l The left-hand side semantics structure to compare.
 * @param r The right-hand side semantics structure to compare.
 * @return True, if the two semantics structures are not equal; false otherwise.
 */
inline bool operator!=(const UntypedImageSemantics& l, const UntypedImageSemantics& r)
{
  return !(l == r);
}


}  // namespace sln

#endif  // SELENE_IMG_DYNAMIC_UNTYPED_LAYOUT_HPP