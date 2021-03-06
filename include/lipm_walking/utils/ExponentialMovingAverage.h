/* 
 * Copyright (c) 2018-2019, CNRS-UM LIRMM
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

/** Exponential Moving Average.
 *
 * This filter can be seen as an integrator:
 *
 *    y(t) = 1/T int_{u=0}^t x(u) e^{(u - t) / T} d{u}
 *
 * with T > 0 a reset period acting as anti-windup. It can also (informally) be
 * interpreted as the average value of the input signal x(t) over the last T
 * seconds. Formally, it represents the amount of time for the smoothed
 * response of a unit input to reach 1-1/e (~63%) of the original signal.
 *
 * See <https://en.wikipedia.org/wiki/Exponential_smoothing>. It is equivalent
 * to a low-pass filter <https://en.wikipedia.org/wiki/Low-pass_filter> applied
 * to the integral of the input signal.
 *
 */
struct ExponentialMovingAverage
{
  static constexpr double MIN_TIME_CONSTANT = 0.01; // [s]

  /** Constructor.
   *
   * \param dt Time in [s] between two readings.
   *
   * \param timeConstant Informally, length of the recent-past window, in [s].
   *
   * \param initValue Initial value of the output average.
   *
   */
  ExponentialMovingAverage(double dt, double timeConstant, const Eigen::Vector3d & initValue = Eigen::Vector3d::Zero())
    : dt_(dt)
  {
    average_ = initValue;
    this->timeConstant(timeConstant);
  }

  /** Append a new reading to the series.
   *
   * \param value New value.
   *
   */
  void append(const Eigen::Vector3d & value)
  {
    average_ += alpha_ * (value - average_);
    if (saturation_ > 0.)
    {
      saturate_();
    }
  }

  /** Evaluate the smoothed statistic.
   *
   */
  const Eigen::Vector3d & eval() const
  {
    return average_;
  }

  /** Set output saturation; disable by providing a negative value.
   *
   * \param limit Output will saturate between -limit and +limit.
   *
   */
  void saturation(double limit)
  {
    saturation_ = limit;
  }

  /** Reset average to zero.
   *
   */
  void setZero()
  {
    average_.setZero();
  }

  /** Get time constant of the filter.
   *
   */
  double timeConstant() const
  {
    return timeConstant_;
  }

  /** Update time constant.
   *
   * \param T New time constant of the filter.
   *
   */
  void timeConstant(double T)
  {
    T = std::max(T, MIN_TIME_CONSTANT);
    alpha_ = 1. - std::exp(-dt_ / T);
    timeConstant_ = T;
  }

private:
  /** Saturate averaged values.
   *
   */
  void saturate_()
  {
    for (unsigned i = 0; i < 3; i++)
    {
      if (average_(i) < -saturation_)
      {
        average_(i) = -saturation_;
      }
      else if (average_(i) > saturation_)
      {
        average_(i) = saturation_;
      }
    }
  }

protected:
  Eigen::Vector3d average_ = Eigen::Vector3d::Zero();
  double alpha_;
  double dt_;
  double timeConstant_;
  double saturation_ = -1.;
};
