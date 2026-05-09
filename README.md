# KRNL Hardware Info

So this is my README. Please read this. It's important, trust me.

KRNL Hardware Info is an experimental low-level hardware info / research project
by a single dev. It is meant to be beginner-friendly, VM-first, and honest about
the risks that come with playing around near kernel-level stuff.

> [!IMPORTANT]
> ALSO READ THE DOCS FOLDER!! ITS ALSO IMPORTANT! READ EVERY README YOU SEE IN
> HERE IF YOU DONT KNOW WHAT YOU'RE DOING!

---

## Community / Updates

[![Discord](https://img.shields.io/badge/Discord-Join%20Server-5865F2?logo=discord&logoColor=white)](https://discord.gg/RkhHNufAQu)

Join the Discord for project updates, discussion, and chaos.

---

## Table of Contents

- [At a Glance](#at-a-glance)
- [What This is / Is Not](#what-this-is--is-not)
- [Quick Notes](#quick-notes)
- [Current Status](#current-status)
- [Safety Warning](#safety-warning)
- [What is KRNL](#what-is-krnl)
- [My Notes for Casual Coders Reading This](#my-notes-for-casual-coders-reading-this)
- [My Notes for Veteran Coders](#my-notes-for-veteran-coders)
- [Planned Features](#planned-features)
- [Build Status](#build-status)

---

## At a Glance

- **Project type:** Windows hardware info / low-level research tool
- **Core idea:** user-mode app + kernel driver
- **Current focus:** CPUID, driver communication, virtualization artifacts
- **Safety model:** VM-first, test-signing only, no bypass/stealth loader stuff
- **Status:** early development, skeleton/docs/build setup are being worked on

--

## What This Is / Is Not

**KRNL is:**

- a beginner-friendly Windows driver learning project
- a hardware telemetry / CPUID research tool
- a VM-first experiment for learning low-level Windows internals

**KRNL is not:**

- a cheat
- a malware project
- a driver-signing bypass tool
- something you should casually run on your daily PC

--

## Quick Notes

- This program is purposefully very beginner friendly.
- This README is gonna be slightly structured too.
- This is a project by a single dev.
- The code isn't obfuscated at all.
- The goal is learning, hardware telemetry, and VM artifact research. Nothing
  sneaky.

---

## Current Status

KRNL Hardware Info is currently a low-level hobby / research project, not a
polished finished product.

The current idea is:

- hardware monitoring
- kernel-level driver experiments
- checking virtualization artifacts that a Hypervisor would leave behind
- learning how this stuff connects without pretending it is magically safe

---

## Safety Warning

> [!WARNING]
> PLEASE dont use this on your Personal PC.

Id recommend using a simple VM to test everything out since if you play games
Anti-Cheats will cry because of unsigned Drivers at a kernel level which i m
using here.

Even though ik the Capabilities KRNL has, use a VM.

---

## What is KRNL

KRNL is a hardware Monitor running on a Kernel level through a driver.

KRNL's main purpose is to help learn about hardware telemetry and the visible
artifacts that virtualization can leave behind.

Ik low-level driver and Hypervisor stuff can get abused if people take it in the
wrong direction. That is not what this repo is for.

Even though ik the Capabilities KRNL has PLEASE dont use this on your Personal
PC.

Id recommend using a simple VM to test everything out since if you play games
Anti-Cheats will cry because of unsigned Drivers at a kernel level which i m
using here.

---

## My Notes for Casual Coders Reading This

Anyone should read this.

Hey you read all that above so you know the basics.

### 1. RUN THIS ON A VM

Why??

Because my driver is very likely to have some kind of security risk hackers can
abuse.

### 2. Why even run this program if the driver may be unsafe?

This tool isnt for OPSEC maniacs. If you are one improve the driver yourself.
This is for people learning how Windows drivers, hardware telemetry, and
virtualization artifacts work. THIS IS NOT AN OPSEC TOOL "Mr. Robot"!!

### 3. Why would i need this if i can find out everything myself?

First of all, if you want to find everything out yourself, do it and delete my
program. Second of all, KRNL makes it easier to find out info about if a
Hypervisor leaves visible artifacts. THIS IS FOR LEARNING AND SECURITY RESEARCH!!

### 4. What if i dont do HV development?

Use this as a Hardware monitor then and if you have one, why did you install
KRNL??

### 5. Are you gonna sign your driver?

No. Do it yourself if your desperate for a (maybe) vulnerable driver thats
signed. For this repo, development means local test-signing in a VM, not
shipping a magical production driver.

### 6. Why would i believe that KRNL isnt a RAT or any other kind malware?

KRNL is open source. Check the code yourself if your that paranoid (tho tbh if
your reading this you may be just that kind of person).

---

## My Notes for Veteran Coders

Bro just read everything in the Casual coders section and your good.

---

## Planned Features

Nothing fake listed here yet. This section exists so future plans can be added
without making the project sound more finished than it is.

---

## Build Status

This project is still early.

| Component | Status |
|---|---|
| **User-mode app** | Builds and launches successfully |
| **Driver connection** | Expected to fail until the driver is built, installed, and started |
| **Kernel driver** | Build/test-signing workflow still in progress |

What exists right now:

- skeleton app/driver/shared code
- docs explaining the intended direction
- Visual Studio / MSBuild project setup started
- beginner notes for WDK, test-signing, and VM testing

What is still rough:

- build setup may still need local Visual Studio / WDK retargeting
- driver work requires the WDK
- loading the driver requires proper signing or test-signing
- VM testing is strongly recommended
- this is not a finished hardware monitor yet
