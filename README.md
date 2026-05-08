# KRNL Hardware Info

So this is my README. Please read this. Its important, trust me.

KRNL Hardware Info is an experimental low-level hardware info / research project
by a single dev. It is meant to be beginner friendly, VM-first, and honest about
the risks that come with playing around near kernel-level stuff.

> [!IMPORTANT]
> ALSO READ THE DOCS FOLDER!! ITS ALSO IMPORTANT! READ EVERY README YOU SEE IN
> HERE IF YOU DONT KNOW WHAT YOUR DOING!

---

## Table of Contents

- [Quick Notes](#quick-notes)
- [Current Status](#current-status)
- [Safety Warning](#safety-warning)
- [What is KRNL](#what-is-krnl)
- [My Notes for Casual Coders Reading This](#my-notes-for-casual-coders-reading-this)
- [My Notes for Veteran Coders](#my-notes-for-veteran-coders)
- [Planned Features](#planned-features)
- [Build Status](#build-status)

---

## Quick Notes

- This program is purposefully very beginner friendly.
- This README is gonna be slightly structured too.
- This is a project by a single dev.
- The code isnt obfuscated at all.
- Maybe just to trick some people this will be obfuscated into simple BASE64.

---

## Current Status

KRNL Hardware Info is currently a low-level hobby / research project, not a
polished finished product.

The current idea is:

- hardware monitoring
- kernel-level driver experiments
- checking traces that a Hypervisor would leave behind
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

KRNL's main purpose is to let you see not just temps but also see traces that a
Hypervisor would leave behind.

Ik what cheat devs could do with this if they actually manage to build a good HV
(Hypervisor).

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

This Tool isnt for OPSEC maniacs. If you are one improve the driver yourself.
This is for people trying to make a VM or Hypervisor stealthy and more
undetectable. THIS IS NOT AN OPSEC TOOL "Mr. Robot"!!

### 3. Why would i need this if i can find out everything myself?

First of all, if you want to find everything out yourself, do it and delete my
program. Second of all, KRNL makes it easier to find out info about if a
Hypervisor can be Detected. THIS IS FOR SECURITY RESEARCH!!

### 4. What if i dont do HV development?

Use this as a Hardware monitor then and if you have one, why did you install
KRNL??

### 5. Are you gonna sign your driver?

No. Do it yourself if your desperate for a (maybe) vulnerable driver thats
signed. Theres plenty on the Internet.

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

Build status is not wired up here yet.

