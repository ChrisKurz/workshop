# SPDX-License-Identifier: Apache-2.0
#
# TrustZone workshop demo application
# nRF54L15 DK + nRF Connect SDK v3.4.0
#
# This is a standard Zephyr/nRF Connect SDK application. Build it with
# `west build`, as described in README.md.

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(trustzone_demo)

target_sources(app PRIVATE src/main.c)
