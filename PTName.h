
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2021.

#ifndef PTNAME_H
#define PTNAME_H

#include <QString>

inline QString PTName ( const QString & TType ,   // Table Type.
                        const QString & PType ) { // Partition Type or GUID.

  typedef const char * const CPtr ;
  typedef struct { CPtr
    T , // partition Table type.
    P , // Partition type / GUID.
    N ; // partition type description.
  } PNRc ;

  //   These constants are hardcoded in "util-linux" and "gptfdisk" packages.
  // "udev" uses this via "libblkid".
  // See also https://wikipedia.org/wiki/GUID_Partition_Table and
  // https://github.com/freemint/freemint

  static CPtr
    DOS = "dos" , SUN = "sun" , SGI = "sgi" ,
    BSD = "bsd" , GPT = "gpt" , MAC = "mac" , TOS = "atari" ;

  static const PNRc PNTbl [ ] = {
    // Master Boot Record.
    { DOS , "0x0"  , "Empty"                     } ,
    { DOS , "0x1"  , "FAT12"                     } ,
    { DOS , "0x2"  , "XENIX root"                } ,
    { DOS , "0x3"  , "XENIX usr"                 } ,
    { DOS , "0x4"  , "FAT16 <32M"                } ,
    { DOS , "0x5"  , "Extended"                  } ,
    { DOS , "0x6"  , "FAT16 >=32M"               } ,
    { DOS , "0x7"  , "HPFS/NTFS/exFAT/QNX"       } ,
    { DOS , "0x8"  , "AIX / SplitDrive"          } ,
    { DOS , "0x9"  , "AIX bootable / Coherent"   } ,
    { DOS , "0xa"  , "OS/2 Boot Manager"         } ,
    { DOS , "0xb"  , "W95 FAT32"                 } ,
    { DOS , "0xc"  , "W95 FAT32 (LBA)"           } ,
    { DOS , "0xe"  , "W95 FAT16 (LBA)"           } ,
    { DOS , "0xf"  , "W95 Ext'd (LBA)"           } ,
    { DOS , "0x10" , "OPUS"                      } ,
    { DOS , "0x11" , "Hidden FAT12"              } ,
    { DOS , "0x12" , "Compaq diagnostics"        } ,
    { DOS , "0x14" , "Hidden FAT16 <32M"         } ,
    { DOS , "0x16" , "Hidden FAT16"              } ,
    { DOS , "0x17" , "Hidden HPFS/NTFS"          } ,
    { DOS , "0x18" , "AST SmartSleep"            } ,
    { DOS , "0x1b" , "Hidden W95 FAT32"          } ,
    { DOS , "0x1c" , "Hidden W95 FAT32 (LBA)"    } ,
    { DOS , "0x1e" , "Hidden W95 FAT16 (LBA)"    } ,
    { DOS , "0x24" , "NEC DOS"                   } ,
    { DOS , "0x27" , "Hidden NTFS WinRE"         } ,
    { DOS , "0x39" , "Plan 9"                    } ,
    { DOS , "0x3c" , "PartitionMagic recovery"   } ,
    { DOS , "0x40" , "Venix 80286"               } ,
    { DOS , "0x41" , "PPC PReP Boot"             } ,
    { DOS , "0x42" , "SFS"                       } ,
    { DOS , "0x4d" , "QNX4.x"                    } ,
    { DOS , "0x4e" , "QNX4.x 2nd part"           } ,
    { DOS , "0x4f" , "QNX4.x 3rd part"           } ,
    { DOS , "0x50" , "OnTrack DiskManager"       } ,
    { DOS , "0x51" , "OnTrack DM6 Aux1 / Novell" } ,
    { DOS , "0x52" , "CP/M / Microport SysV/AT"  } ,
    { DOS , "0x53" , "OnTrack DiskManager6 Aux3" } ,
    { DOS , "0x54" , "OnTrack DiskManager 6"     } ,
    { DOS , "0x55" , "EZ-Drive"                  } ,
    { DOS , "0x56" , "Golden Bow"                } ,
    { DOS , "0x5c" , "Priam Edisk"               } ,
    { DOS , "0x61" , "SpeedStor"                 } ,
    { DOS , "0x63" , "GNU HURD / Mach / SysV"    } ,
    { DOS , "0x64" , "Novell Netware 286"        } ,
    { DOS , "0x65" , "Novell Netware 386"        } ,
    { DOS , "0x70" , "DiskSecure Multi-Boot"     } ,
    { DOS , "0x75" , "PC/IX"                     } ,
    { DOS , "0x80" , "Old Minix (<=1.4a)"        } ,
    { DOS , "0x81" , "Minix(>=1.4b) / old Linux" } ,
    { DOS , "0x82" , "Linux swap / Solaris"      } ,
    { DOS , "0x83" , "Linux"                     } ,
    { DOS , "0x84" , "OS/2 hidden / Intel hib."  } ,
    { DOS , "0x85" , "Linux extended"            } ,
    { DOS , "0x86" , "NTFS volume set"           } ,
    { DOS , "0x87" , "NTFS volume set"           } ,
    { DOS , "0x88" , "Linux plaintext"           } ,
    { DOS , "0x8e" , "Linux LVM"                 } ,
    { DOS , "0x93" , "Amoeba"                    } ,
    { DOS , "0x94" , "Amoeba Bad Block Table"    } ,
    { DOS , "0x9f" , "BSD/OS"                    } ,
    { DOS , "0xa0" , "IBM Thinkpad hibernation"  } ,
    { DOS , "0xa5" , "FreeBSD"                   } ,
    { DOS , "0xa6" , "OpenBSD"                   } ,
    { DOS , "0xa7" , "NeXTSTEP"                  } ,
    { DOS , "0xa8" , "Darwin UFS"                } ,
    { DOS , "0xa9" , "NetBSD"                    } ,
    { DOS , "0xab" , "Darwin boot"               } ,
    { DOS , "0xaf" , "HFS / HFS+"                } ,
    { DOS , "0xb7" , "BSDI fs"                   } ,
    { DOS , "0xb8" , "BSDI swap"                 } ,
    { DOS , "0xbb" , "Boot Wizard hidden"        } ,
    { DOS , "0xbc" , "Acronis FAT32 LBA"         } ,
    { DOS , "0xbe" , "Solaris boot"              } ,
    { DOS , "0xbf" , "Solaris"                   } ,
    { DOS , "0xc1" , "DRDOS/sec (FAT-12)"        } ,
    { DOS , "0xc4" , "DRDOS/sec (FAT-16 < 32M)"  } ,
    { DOS , "0xc6" , "DRDOS/sec (FAT-16)"        } ,
    { DOS , "0xc7" , "Syrinx"                    } ,
    { DOS , "0xda" , "Non-FS data"               } ,
    { DOS , "0xdb" , "CP/M / CTOS / Concurrent"  } ,
    { DOS , "0xde" , "Dell PowerEdge Utility"    } ,
    { DOS , "0xdf" , "BootIt EMBRM"              } ,
    { DOS , "0xe1" , "DOS access / SpeedStor"    } ,
    { DOS , "0xe3" , "DOS R/O / SpeedStor"       } ,
    { DOS , "0xe4" , "SpeedStor FAT16 extended"  } ,
    { DOS , "0xea" , "Rufus alignment"           } ,
    { DOS , "0xeb" , "BeOS fs"                   } ,
    { DOS , "0xee" , "GPT"                       } ,
    { DOS , "0xef" , "EFI (FAT-12/16/32)"        } ,
    { DOS , "0xf0" , "Linux/PA-RISC boot"        } ,
    { DOS , "0xf1" , "SpeedStor"                 } ,
    { DOS , "0xf4" , "SpeedStor large"           } ,
    { DOS , "0xf2" , "DOS secondary"             } ,
    { DOS , "0xfb" , "VMware VMFS"               } ,
    { DOS , "0xfc" , "VMware VMKCORE"            } ,
    { DOS , "0xfd" , "Linux raid autodetect"     } ,
    { DOS , "0xfe" , "LANstep / SpeedStor"       } ,
    { DOS , "0xff" , "Xenix Bad Block Table"     } ,
    //
    { SUN , "0x0"  , "Unassigned"                } ,
    { SUN , "0x1"  , "Boot"                      } ,
    { SUN , "0x2"  , "SunOS root"                } ,
    { SUN , "0x3"  , "SunOS swap"                } ,
    { SUN , "0x4"  , "SunOS usr"                 } ,
    { SUN , "0x5"  , "Whole disk"                } ,
    { SUN , "0x6"  , "SunOS stand"               } ,
    { SUN , "0x7"  , "SunOS var"                 } ,
    { SUN , "0x8"  , "SunOS home"                } ,
    { SUN , "0x9"  , "SunOS alt sectors"         } ,
    { SUN , "0xa"  , "SunOS cachefs"             } ,
    { SUN , "0xb"  , "SunOS reserved"            } ,
    { SUN , "0x82" , "Linux swap"                } ,
    { SUN , "0x83" , "Linux native"              } ,
    { SUN , "0x8e" , "Linux LVM"                 } ,
    { SUN , "0xfd" , "Linux raid autodetect"     } ,
    // IRIX.
    { SGI , "0x0"  , "SGI volhdr"                } ,
    { SGI , "0x1"  , "SGI trkrepl"               } ,
    { SGI , "0x2"  , "SGI secrepl"               } ,
    { SGI , "0x3"  , "SGI raw"                   } ,
    { SGI , "0x4"  , "SGI bsd"                   } ,
    { SGI , "0x5"  , "SGI sysv"                  } ,
    { SGI , "0x6"  , "SGI volume"                } ,
    { SGI , "0x7"  , "SGI efs"                   } ,
    { SGI , "0x8"  , "SGI lvol"                  } ,
    { SGI , "0x9"  , "SGI rlvol"                 } ,
    { SGI , "0xa"  , "SGI xfs"                   } ,
    { SGI , "0xb"  , "SGI xfslog"                } ,
    { SGI , "0xc"  , "SGI xlv"                   } ,
    { SGI , "0xd"  , "SGI xvm"                   } ,
    { SGI , "0x82" , "Linux swap"                } ,
    { SGI , "0x83" , "Linux native"              } ,
    { SGI , "0x8e" , "Linux LVM"                 } ,
    { SGI , "0xfd" , "Linux RAID"                } ,
    // BSD Disklabel.
    { BSD , "0x0"  , "unused"                    } ,
    { BSD , "0x1"  , "swap"                      } ,
    { BSD , "0x2"  , "Version 6"                 } ,
    { BSD , "0x3"  , "Version 7"                 } ,
    { BSD , "0x4"  , "System V"                  } ,
    { BSD , "0x5"  , "4.1BSD"                    } ,
    { BSD , "0x6"  , "Eighth Edition"            } ,
    { BSD , "0x7"  , "4.2BSD"                    } ,
    { BSD , "0x8"  , "MS-DOS"                    } ,
    { BSD , "0x9"  , "4.4LFS"                    } ,
    { BSD , "0xa"  , "unknown"                   } ,
    { BSD , "0xb"  , "HPFS"                      } ,
    { BSD , "0xc"  , "ISO-9660"                  } ,
    { BSD , "0xd"  , "boot"                      } ,
    { BSD , "0xe"  , "ADOS"                      } ,
    { BSD , "0xf"  , "HFS"                       } ,
    { BSD , "0x10" , "AdvFS"                     } ,
    // Atari TOS
    { TOS , "XGM" , "TOS Extended"               } ,
    { TOS , "GEM" , "GEMDOS FAT16 (<32 MB)"      } ,
    { TOS , "BGM" , "GEMDOS FAT16 (>=32MB)"      } ,
    { TOS , "F32" , "TOS FAT32 (Win95)"          } ,
    { TOS , "LNX" , "Linux native (ext2)"        } ,
    { TOS , "MAC" , "Mac HFS"                    } ,
    { TOS , "MIX" , "Minix"                      } ,
    { TOS , "MNX" , "Minix"                      } ,
    { TOS , "RAW" , "Unspecified (RAW)"          } ,
    { TOS , "UNX" , "Atari SysV R4 Unix"         } ,
    { TOS , "SWP" , "Swap"                       } ,
    // GUID Partition Table.
    // N/A
    { GPT , "00000000-0000-0000-0000-000000000000" ,
                     "Unused entry"              } ,
    { GPT , "c12a7328-f81f-11d2-ba4b-00a0c93ec93b" ,
                     "EFI System"                } ,
    { GPT , "024dee41-33e7-11d3-9d69-0008c781f39f" ,
                     "MBR partition scheme"      } ,
    { GPT , "d3bfe2de-3daf-11df-ba40-e3a556d89593" ,
                     "Intel Fast Flash"          } ,
    { GPT , "21686148-6449-6e6f-744e-656564454649" ,
                     "BIOS boot"                 } ,
    { GPT , "f4019732-066e-4e12-8273-346c5641494f" ,
                     "Sony boot"                 } ,
    { GPT , "bfbfafe7-a34f-448a-9a5b-6213eb736c22" ,
                     "Lenovo boot"               } ,
    // Microsoft Windows
    { GPT , "e3c9e316-0b5c-4db8-817d-f92df00215ae" ,
                     "Microsoft reserved"        } ,
    { GPT , "ebd0a0a2-b9e5-4433-87c0-68b6b72699c7" ,
                     "Microsoft basic data"      } ,
    { GPT , "5808c8aa-7e8f-42e0-85d2-e1e90434cfb3" ,
                     "Microsoft LDM metadata"    } ,
    { GPT , "af9b60a0-1431-4f62-bc68-3311714a69ad" ,
                     "Microsoft LDM data"        } ,
    { GPT , "de94bba4-06d1-4d40-a16a-bfd50179d6ac" ,
                     "Windows recovery environ." } ,
    { GPT , "37affc90-ef7d-4e96-91c3-2d7ae055b174" ,
                     "IBM General Parallel Fs"   } ,
    { GPT , "e75caf8f-f680-4cee-afa3-b001e56efc2d" ,
                     "Microsoft Storage Spaces"  } ,
    { GPT , "558D43C5-A1AC-43C0-AAC8-D1472B2923D1" ,
                     "Microsoft Storage Replica" } ,
    // HP-UX
    { GPT , "75894c1e-3aeb-11d3-b7c1-7b03a0000000" ,
                     "HP-UX data"                } ,
    { GPT , "e2a1e728-32e3-11d6-a682-7b03a0000000" ,
                     "HP-UX service"             } ,
    // Linux
    { GPT , "0657fd6d-a4ab-43c4-84e5-0933c84b4f4f" ,
                     "Linux swap"                } ,
    { GPT , "0fc63daf-8483-4772-8e79-3d69d8477de4" ,
                     "Linux filesystem"          } ,
    { GPT , "3b8f8425-20e0-4f3b-907f-1a25a76f98e8" ,
                     "Linux server data"         } ,
    { GPT , "44479540-f297-41b2-9af7-d131d5f0458a" ,
                     "Linux root (x86)"          } ,
    { GPT , "69dad710-2ce4-4e3c-b16c-21a1d49abed3" ,
                     "Linux root (ARM)"          } ,
    { GPT , "4f68bce3-e8cd-4db1-96e7-fbcaf984b709" ,
                     "Linux root (x86-64)"       } ,
    { GPT , "b921b045-1df0-41c3-af44-4c6f280d3fae" ,
                     "Linux root (ARM-64)"       } ,
    { GPT , "993d8d3d-f80e-4225-855a-9daf8ed7ea97" ,
                     "Linux root (IA-64)"        } ,
    { GPT , "8da63339-0007-60c0-c436-083ac8230908" ,
                     "Linux reserved"            } ,
    { GPT , "933ac7e1-2eb4-4f13-b844-0e14e2aef915" ,
                     "Linux home"                } ,
    { GPT , "a19d880f-05fc-4d3b-a006-743f0f84911e" ,
                     "Linux RAID"                } ,
    { GPT , "e6d6d379-f507-44c2-a23c-238f2a3df928" ,
                     "Linux LVM"                 } ,
    { GPT , "7ffec5c9-2d00-49b7-8941-3ea10a5586b7" ,
                     "Linux plain dm-crypt"      } ,
    { GPT , "ca7d7ccb-63ed-4c53-861c-1742536059cc" ,
                     "Linux LUKS"                } ,
    { GPT , "d13c5d3b-b5d1-422a-b29f-9454fdc89d76" ,
                     "Linux x86 root verity"     } ,
    { GPT , "2c7357ed-ebd2-46d9-aec1-23d437ec2bf5" ,
                     "Linux x86-64 root verity"  } ,
    { GPT , "7386cdf2-203c-47a9-a498-f2ecce45a2d6" ,
                     "Linux ARM32 root verity"   } ,
    { GPT , "df3300ce-d69f-4c92-978c-9bfb0f38d820" ,
                     "Linux ARM64 root verity"   } ,
    { GPT , "86ed10d5-b607-45bb-8957-d350f23d0571" ,
                     "Linux IA-64 root verity"   } ,
    { GPT , "8f461b0d-14ee-4e81-9aa9-049b6fb97abd" ,
                     "Linux x86 /usr verity"     } ,
    { GPT , "77ff5f63-e7b6-4633-acf4-1565b864c0e6" ,
                     "Linux x86-64 /usr verity"  } ,
    { GPT , "c215d751-7bcd-4649-be90-6627490a4c05" ,
                     "Linux ARM32 /usr verity"   } ,
    { GPT , "6e11a4e7-fbca-4ded-b9e9-e1a512bb664e" ,
                     "Linux ARM64 /usr verity"   } ,
    { GPT , "6a491e03-3be7-4545-8e38-83320e0ea880" ,
                     "Linux IA-64 /usr verity"   } ,
    // Linux (auto-mounted by systemd)
    { GPT , "4d21b016-b534-45c2-a9fb-5c16e091fd2d" ,
                     "Linux /var"                } ,
    { GPT , "7ec6f557-3bc5-4aca-b293-16ef5df639d1" ,
                     "Linux /var/tmp"            } ,
    { GPT , "75250d76-8cc6-458e-bd66-bd47cc81a812" ,
                     "Linux x86 /usr "           } ,
    { GPT , "8484680c-9521-48c6-9c11-b0720656f69e" ,
                     "Linux x86-64 /usr"         } ,
    { GPT , "7d0359a3-02b3-4f0a-865c-654403e70625" ,
                     "Linux ARM32 /usr"          } ,
    { GPT , "b0e01050-ee5f-4390-949a-9101b17104e9" ,
                     "Linux ARM64 /usr"          } ,
    { GPT , "4301d2a6-4e3b-4b2a-bb94-9e0b2c4225ea" ,
                     "Linux IA-64 /usr"          } ,
    { GPT , "773f91ef-66d4-49b5-bd83-d683bf40ad16" ,
                     "Linux user's home"         } ,
    // FreeBSD
    { GPT , "516e7cb4-6ecf-11d6-8ff8-00022d09712b" ,
                     "FreeBSD data"              } ,
    { GPT , "83bd6b9d-7f41-11dc-be0b-001560b84f0f" ,
                     "FreeBSD boot"              } ,
    { GPT , "516e7cb5-6ecf-11d6-8ff8-00022d09712b" ,
                     "FreeBSD swap"              } ,
    { GPT , "516e7cb6-6ecf-11d6-8ff8-00022d09712b" ,
                     "FreeBSD UFS"               } ,
    { GPT , "516e7cba-6ecf-11d6-8ff8-00022d09712b" ,
                     "FreeBSD ZFS"               } ,
    { GPT , "516e7cb8-6ecf-11d6-8ff8-00022d09712b" ,
                     "FreeBSD Vinum"             } ,
    // Apple/MacOS
    { GPT , "48465300-0000-11aa-aa11-00306543ecac" ,
                     "Apple HFS/HFS+"            } ,
    { GPT , "55465300-0000-11aa-aa11-00306543ecac" ,
                     "Apple UFS"                 } ,
    { GPT , "52414944-0000-11aa-aa11-00306543ecac" ,
                     "Apple RAID"                } ,
    { GPT , "52414944-5f4f-11aa-aa11-00306543ecac" ,
                     "Apple RAID offline"        } ,
    { GPT , "426f6f74-0000-11aa-aa11-00306543ecac" ,
                     "Apple boot"                } ,
    { GPT , "4c616265-6c00-11aa-aa11-00306543ecac" ,
                     "Apple label"               } ,
    { GPT , "5265636f-7665-11aa-aa11-00306543ecac" ,
                     "Apple TV recovery"         } ,
    { GPT , "53746f72-6167-11aa-aa11-00306543ecac" ,
                     "Apple Core storage"        } ,
    { GPT , "7c3457ef-0000-11aa-aa11-00306543ecac" ,
                     "Apple APFS"                } ,
    { GPT , "b6fa30da-92d2-4a9a-96f1-871ec6486200" ,
                     "MacOS SoftRAID_Status"     } ,
    { GPT , "2e313465-19b9-463f-8126-8a7993773801" ,
                     "MacOS SoftRAID_Scratch"    } ,
    { GPT , "fa709c7e-65b1-4593-bfd5-e71d61de9b02" ,
                     "MacOS SoftRAID_Volume"     } ,
    { GPT , "bbba6df5-f46f-4a89-8f59-8765b2727503" ,
                     "MacOS SoftRAID_Cache"      } ,
    // Solaris
    { GPT , "6a82cb45-1dd2-11b2-99a6-080020736631" ,
                     "Solaris boot"              } ,
    { GPT , "6a85cf4d-1dd2-11b2-99a6-080020736631" ,
                     "Solaris root"              } ,
    { GPT , "6a898cc3-1dd2-11b2-99a6-080020736631" ,
                     "Solaris /usr & Apple ZFS"  } ,
    { GPT , "6a87c46f-1dd2-11b2-99a6-080020736631" ,
                     "Solaris swap"              } ,
    { GPT , "6a8b642b-1dd2-11b2-99a6-080020736631" ,
                     "Solaris backup"            } ,
    { GPT , "6a8ef2e9-1dd2-11b2-99a6-080020736631" ,
                     "Solaris /var"              } ,
    { GPT , "6a90ba39-1dd2-11b2-99a6-080020736631" ,
                     "Solaris /home"             } ,
    { GPT , "6a9283a5-1dd2-11b2-99a6-080020736631" ,
                     "Solaris alternate sectors" } ,
    { GPT , "6a945a3b-1dd2-11b2-99a6-080020736631" ,
                     "Solaris reserved 1"        } ,
    { GPT , "6a9630d1-1dd2-11b2-99a6-080020736631" ,
                     "Solaris reserved 2"        } ,
    { GPT , "6a980767-1dd2-11b2-99a6-080020736631" ,
                     "Solaris reserved 3"        } ,
    { GPT , "6a96237f-1dd2-11b2-99a6-080020736631" ,
                     "Solaris reserved 4"        } ,
    { GPT , "6a8d2ac7-1dd2-11b2-99a6-080020736631" ,
                     "Solaris reserved 5"        } ,
    // NetBSD
    { GPT , "49f48d32-b10e-11dc-b99b-0019d1879648" ,
                     "NetBSD swap"               } ,
    { GPT , "49f48d5a-b10e-11dc-b99b-0019d1879648" ,
                     "NetBSD FFS"                } ,
    { GPT , "49f48d82-b10e-11dc-b99b-0019d1879648" ,
                     "NetBSD LFS"                } ,
    { GPT , "2db519c4-b10f-11dc-b99b-0019d1879648" ,
                     "NetBSD concatenated"       } ,
    { GPT , "2db519ec-b10f-11dc-b99b-0019d1879648" ,
                     "NetBSD encrypted"          } ,
    { GPT , "49f48daa-b10e-11dc-b99b-0019d1879648" ,
                     "NetBSD RAID"               } ,
    // ChromeOS
    { GPT , "fe3a2a5d-4f32-41a7-b725-accc3285a309" ,
                     "ChromeOS kernel"           } ,
    { GPT , "3cb8e202-3b7e-47dd-8a3c-7ff2a13cfcec" ,
                     "ChromeOS root fs"          } ,
    { GPT , "2e0a753d-9e48-43b0-8337-b15192cb1b5e" ,
                     "ChromeOS reserved"         } ,
    // Container Linux
    { GPT , "3884dd41-8582-4404-b9a8-e9b84f2df50e" ,
                     "CoreOS Resizable rootfs"   } ,
    { GPT , "5dfbf5f4-2848-4bac-aa5e-0d9a20b745a6" ,
                     "CoreOS /usr"               } ,
    { GPT , "be9067b9-ea49-4f15-b4f6-f36f8c9e1818" ,
                     "CoreOS Root FS on RAID"    } ,
    { GPT , "c95dc21a-df0e-4340-8d7b-26cbfa9a03e0" ,
                     "CoreOS OEM customizations" } ,
    // Haiku
    { GPT , "42465331-3ba3-10f1-802a-4861696b7521" ,
                     "Haiku BFS"                 } ,
    // Midnight BSD
    { GPT , "85d5e45a-237c-11e1-b4b3-e89a8f7fc3a7" ,
                     "MidnightBSD data"          } ,
    { GPT , "85d5e45e-237c-11e1-b4b3-e89a8f7fc3a7" ,
                     "MidnightBSD boot"          } ,
    { GPT , "85d5e45b-237c-11e1-b4b3-e89a8f7fc3a7" ,
                     "MidnightBSD swap"          } ,
    { GPT , "0394ef8b-237e-11e1-b4b3-e89a8f7fc3a7" ,
                     "MidnightBSD UFS"           } ,
    { GPT , "85d5e45d-237c-11e1-b4b3-e89a8f7fc3a7" ,
                     "MidnightBSD ZFS"           } ,
    { GPT , "85d5e45c-237c-11e1-b4b3-e89a8f7fc3a7" ,
                     "MidnightBSD Vinum"         } ,
    // Ceph
    { GPT , "45b0969e-9b03-4f30-b4c6-b4b80ceff106" ,
                     "Ceph Journal"              } ,
    { GPT , "45b0969e-9b03-4f30-b4c6-5ec00ceff106" ,
                     "Ceph Encrypted Journal"    } ,
    { GPT , "4fbd7e29-9d25-41b8-afd0-062c0ceff05d" ,
                     "Ceph OSD"                  } ,
    { GPT , "4fbd7e29-9d25-41b8-afd0-5ec00ceff05d" ,
                     "Ceph crypt OSD"            } ,
    { GPT , "89c57f98-2fe5-4dc0-89c1-f3ad0ceff2be" ,
                     "Ceph disk in creation"     } ,
    { GPT , "89c57f98-2fe5-4dc0-89c1-5ec00ceff2be" ,
                     "Ceph crypt disk in creat." } ,
    { GPT , "cafecafe-9b03-4f30-b4c6-b4b80ceff106" ,
                     "Ceph Block"                } ,
    { GPT , "30cd0809-c2b2-499c-8879-2d6b78529876" ,
                     "Ceph Block DB"             } ,
    { GPT , "5ce17fce-4087-4169-b7ff-056cc58473f9" ,
                     "Ceph Block WA log"         } ,
    { GPT , "fb3aabf9-d25f-47cc-bf5e-721d1816496b" ,
                     "Ceph Lockbox"              } ,
    { GPT , "4fbd7e29-8ae0-4982-bf9d-5a8d867af560" ,
                     "Ceph Multipath OSD"        } ,
    { GPT , "45b0969e-8ae0-4982-bf9d-5a8d867af560" ,
                     "Ceph Multipath journal"    } ,
    { GPT , "cafecafe-8ae0-4982-bf9d-5a8d867af560" ,
                     "Ceph Multipath block 1"    } ,
    { GPT , "7f4a666a-16f3-47a2-8445-152ef4d03f6c" ,
                     "Ceph Multipath block 2"    } ,
    { GPT , "ec6d6385-e346-45dc-be91-da2a7c8b3261" ,
                     "Ceph Multipath block DB"   } ,
    { GPT , "01b41e1b-002a-453c-9f17-88793989ff8f" ,
                     "Ceph Multipath WA log"     } ,
    { GPT , "cafecafe-9b03-4f30-b4c6-5ec00ceff106" ,
                     "Ceph dm-crypt block"       } ,
    { GPT , "93b0052d-02d9-4d8a-a43b-33a3ee4dfbc3" ,
                     "Ceph dm-crypt block DB"    } ,
    { GPT , "306e8683-4fe2-4330-b7c0-00a917c16966" ,
                     "Ceph dm-crypt WA log"      } ,
    { GPT , "45b0969e-9b03-4f30-b4c6-35865ceff106" ,
                     "Ceph LUKS journal"         } ,
    { GPT , "cafecafe-9b03-4f30-b4c6-35865ceff106" ,
                     "Ceph LUKS block"           } ,
    { GPT , "166418da-c469-4022-adf4-b30afd37f176" ,
                     "Ceph LUKS block DB"        } ,
    { GPT , "86a32090-3647-40b9-bbbd-38d8c573aa86" ,
                     "Ceph LUKS block WA log"    } ,
    { GPT , "4fbd7e29-9d25-41b8-afd0-35865ceff05d" ,
                     "Ceph LUKS OSD"             } ,
    // OpenBSD
    { GPT , "824cc7a0-36a8-11e3-890a-952519ad3f61" ,
                     "OpenBSD data"              } ,
    // QNX
    { GPT , "cef5a9ad-73bc-4601-89f3-cdeeeee321a1" ,
                     "Power Safe (QNX6) FS"      } ,
    // Plan 9
    { GPT , "c91818f9-8025-47af-89d2-f030d7000c2c" ,
                     "Plan 9"                    } ,
    // VMWare
    { GPT , "9198effc-31c0-11db-8f78-000c2911d1b8" ,
                     "VMWare reserved"           } ,
    { GPT , "381cfccc-7288-11e0-92ee-000c2911d0b2" ,
                     "VMWare Virtual SAN"        } ,
    { GPT , "9d275380-40ad-11db-bf97-000c2911d1b8" ,
                     "VMWare kcore crash prot."  } ,
    { GPT , "aa31e02a-400f-11db-9590-000c2911d1b8" ,
                     "VMWare VMFS"               } ,
    { GPT , "77719a0c-a4a0-11e3-a47e-000c29745a24" ,
                     "VMWare Virsto"             } ,
    // Android-IA
    { GPT , "2568845d-2332-4675-bc39-8fa5a4748d15" ,
                     "Android-IA Bootloader"     } ,
    { GPT , "114eaffe-1552-4022-b26e-9b053604cf84" ,
                     "Android-IA Bootloader 2"   } ,
    { GPT , "49a4d17f-93a3-45c1-a0de-f50b2ebe2599" ,
                     "Android-IA Boot"           } ,
    { GPT , "4177c722-9e92-4aab-8644-43502bfd5506" ,
                     "Android-IA Recovery"       } ,
    { GPT , "ef32a33b-a409-486c-9141-9ffb711f6266" ,
                     "Android-IA Misc"           } ,
    { GPT , "20ac26be-20b7-11e3-84c5-6cfdb94711e9" ,
                     "Android-IA Metadata"       } ,
    { GPT , "38f428e6-d326-425d-9140-6e0ea133647c" ,
                     "Android-IA System"         } ,
    { GPT , "a893ef21-e428-470a-9e55-0668fd91a2d9" ,
                     "Android-IA Cache"          } ,
    { GPT , "dc76dda9-5ac1-491c-af42-a82591580c0d" ,
                     "Android-IA Data"           } ,
    { GPT , "ebc597d0-2053-4b15-8b64-e0aac75f4db1" ,
                     "Android-IA Persistent"     } ,
    { GPT , "c5a0aeec-13ea-11e5-a1b1-001e67ca0c3c" ,
                     "Android-IA Vendor"         } ,
    { GPT , "bd59408b-4514-490d-bf12-9878d963f378" ,
                     "Android-IA Config"         } ,
    { GPT , "8f68cc74-c5e5-48da-be91-a0c8c15e9c80" ,
                     "Android-IA Factory"        } ,
    { GPT , "9fdaa6ef-4b3f-40d2-ba8d-bff16bfb887b" ,
                     "Android-IA Factory (alt)"  } ,
    { GPT , "767941d0-2085-11e3-ad3b-6cfdb94711e9" ,
                     "Android-IA Fastboot/Tert." } ,
    { GPT , "ac6d7924-eb71-4df8-b48d-e267b27148ff" ,
                     "Android-IA OEM"            } ,
    // Android 6.0+
    { GPT , "19a710a2-b3ca-11e4-b026-10604b889dcf" ,
                     "Android Meta"              } ,
    { GPT , "193d1ea4-b3ca-11e4-b075-10604b889dcf" ,
                     "Android EXT"               } ,
    { GPT , "dea0ba2c-cbdd-4805-b4f9-f428251c3e98" ,
                     "Android SBL1"              } ,
    { GPT , "8c6b52ad-8a9e-4398-ad09-ae916e53ae2d" ,
                     "Android SBL2"              } ,
    { GPT , "05e044df-92f1-4325-b69e-374a82e97d6e" ,
                     "Android SBL3"              } ,
    { GPT , "400ffdcd-22e0-47e7-9a23-f16ed9382388" ,
                      "Android APPSBL"           } ,
    { GPT , "a053aa7f-40b8-4b1c-ba08-2f68ac71a4f4" ,
                     "Android QSEE/tz"           } ,
    { GPT , "e1a6a689-0c8d-4cc6-b4e8-55a4320fbd8a" ,
                     "Android QHEE/hyp"          } ,
    { GPT , "098df793-d712-413d-9d4e-89d711772228" ,
                     "Android RPM"               } ,
    { GPT , "d4e0d938-b7fa-48c1-9d21-bc5ed5c4b203" ,
                     "Android WDOG debug/sdi"    } ,
    { GPT , "20a0c19c-286a-42fa-9ce7-f64c3226a794" ,
                     "Android DDR"               } ,
    { GPT , "a19f205f-ccd8-4b6d-8f1e-2d9bc24cffb1" ,
                     "Android CDT"               } ,
    { GPT , "66c9b323-f7fc-48b6-bf96-6f32e335a428" ,
                     "Android RAM dump"          } ,
    { GPT , "303e6ac3-af15-4c54-9e9b-d9a8fbecf401" ,
                     "Android SEC"               } ,
    { GPT , "c00eef24-7709-43d6-9799-dd2b411e7a3c" ,
                     "Android PMIC"              } ,
    { GPT , "82acc91f-357c-4a68-9c8f-689e1b1a23a1" ,
                     "Android misc 1"            } ,
    { GPT , "e2802d54-0545-e8a1-a1e8-c7a3e245acd4" ,
                     "Android misc 2"            } ,
    { GPT , "65addcf4-0c5c-4d9a-ac2d-d90b5cbfcd03" ,
                     "Android device info"       } ,
    { GPT , "e6e98da2-e22a-4d12-ab33-169e7deaa507" ,
                     "Android APDP"              } ,
    { GPT , "ed9e8101-05fa-46b7-82aa-8d58770d200b" ,
                     "Android MSADP"             } ,
    { GPT , "11406f35-1173-4869-807b-27df71802812" ,
                     "Android DPO"               } ,
    { GPT , "9d72d4e4-9958-42da-ac26-bea7a90b0434" ,
                     "Android recovery 2"        } ,
    { GPT , "6c95e238-e343-4ba8-b489-8681ed22ad0b" ,
                     "Android persist"           } ,
    { GPT , "ebbeadaf-22c9-e33b-8f5d-0e81686a68cb" ,
                     "Android modem ST1"         } ,
    { GPT , "0a288b1f-22c9-e33b-8f5d-0e81686a68cb" ,
                     "Android modem ST2"         } ,
    { GPT , "57b90a16-22c9-e33b-8f5d-0e81686a68cb" ,
                     "Android FSC"               } ,
    { GPT , "638ff8e2-22c9-e33b-8f5d-0e81686a68cb" ,
                     "Android FSG 1"             } ,
    { GPT , "2013373e-1ac4-4131-bfd8-b6a7ac638772" ,
                     "Android FSG 2"             } ,
    { GPT , "2c86e742-745e-4fdd-bfd8-b6a7ac638772" ,
                     "Android SSD"               } ,
    { GPT , "de7d4029-0f5b-41c8-ae7e-f6c023a02b33" ,
                     "Android keystore"          } ,
    { GPT , "323ef595-af7a-4afa-8060-97be72841bb9" ,
                     "Android encrypt"           } ,
    { GPT , "45864011-cf89-46e6-a445-85262e065604" ,
                     "Android EKSST"             } ,
    { GPT , "8ed8ae95-597f-4c8a-a5bd-a7ff8e4dfaa9" ,
                     "Android RCT"               } ,
    { GPT , "df24e5ed-8c96-4b86-b00b-79667dc6de11" ,
                     "Android spare1"            } ,
    { GPT , "7c29d3ad-78b9-452e-9deb-d098d542f092" ,
                     "Android spare2"            } ,
    { GPT , "379d107e-229e-499d-ad4f-61f5bcf87bd4" ,
                     "Android spare3"            } ,
    { GPT , "0dea65e5-a676-4cdf-823c-77568b577ed5" ,
                     "Android spare4"            } ,
    { GPT , "4627ae27-cfef-48a1-88fe-99c3509ade26" ,
                     "Android raw resources"     } ,
    { GPT , "20117f86-e985-4357-b9ee-374bc1d8487d" ,
                     "Android boot 2"            } ,
    { GPT , "86a7cb80-84e1-408c-99ab-694f1a410fc7" ,
                     "Android FOTA"              } ,
    { GPT , "97d7b011-54da-4835-b3c4-917ad6e73d74" ,
                     "Android system 2"          } ,
    { GPT , "5594c694-c871-4b5f-90b1-690a6f68e0f7" ,
                     "Android cache"             } ,
    { GPT , "1b81e7e6-f50d-419b-a739-2aeef8da3335" ,
                     "Android user data"         } ,
    { GPT , "98523ec6-90fe-4c67-b50a-0fc59ed6f56d" ,
                     "Android LG adv. flasher"   } ,
    { GPT , "2644bcc0-f36a-4792-9533-1738bed53ee3" ,
                     "Android PG1FS"             } ,
    { GPT , "dd7c91e9-38c9-45c5-8a12-4a80f7e14057" ,
                     "Android PG2FS"             } ,
    { GPT , "7696d5b6-43fd-4664-a228-c563c4a1e8cc" ,
                     "Android board info"        } ,
    { GPT , "0d802d54-058d-4a20-ad2d-c7a362ceacd4" ,
                     "Android MFG"               } ,
    { GPT , "10a0c19c-516a-5444-5ce3-664c3226a794" ,
                     "Android limits"            } ,
    // Open Network Install Environment
    { GPT , "7412f7d5-a156-4b13-81dc-867174929325" ,
                     "ONIE boot"                 } ,
    { GPT , "d4e6e2cd-4469-46f3-b5cb-1bff57afc149" ,
                     "ONIE config"               } ,
    // PowerPC
    { GPT , "9e1a2d38-c612-4316-aa26-8b49521e5a8b" ,
                     "PowerPC PReP boot"         } ,
    // freedesktop.org
    { GPT , "bc13c2ff-59e6-4262-a352-b275fd6f7172" ,
                     "Shared bootloader config." } ,
    // Atari TOS
    { GPT , "734e5afe-f61a-11e6-bc64-92361f002671" ,
                     "Atari Basic data"          } ,
    // VeraCrypt
    { GPT , "8c8f8eff-ac95-4770-814a-21994f2dbc8f" ,
                     "Veracrypt data"            } ,
    // OS/2
    { GPT , "90b6ff38-b98f-4358-a21f-48f35b4a8ad3" ,
                     "ArcaOS Type 1"             } ,
    // Storage Performance Development Kit
    { GPT , "7c5222bd-8f5d-4087-9c00-bf9843c7b58c" ,
                     "SPDK block device"         } ,
    // Barebox
    { GPT , "4778ED65-BF42-45FA-9C5B-287A1DC4AAB1" ,
                     "Barebox boot loader"       } ,
    // Acronis
    { GPT , "0311fc50-01ca-4725-ad77-9adbb20ace98" ,
                     "Acronis Secure Zone"       } ,
    // HiFive Unleashed
    { GPT , "2e54b353-1271-4842-806f-e436d6af6985" ,
                     "HiFive Unleashed BBL"      } ,
    { GPT , "5b193300-fc78-40cd-8002-e86c45580b47" ,
                     "HiFive Unleashed FSBL"     } ,
  } ;

  QString N ; // Resulting Name.

  //   The "endsWith" is used because the value of "ID_PART_TABLE_TYPE" and
  // "ID_PART_ENTRY_SCHEME" properties can be prefixed.

  if ( ! TType . endsWith ( MAC ) ) {
    static const int PNTSz = sizeof ( PNTbl ) / sizeof ( PNRc ) ;
    for ( int I = 0 ; I < PNTSz && N . isEmpty ( ) ; ++ I ) {
      const PNRc & R = PNTbl [ I ] ;
    if ( TType . endsWith ( R . T ) && PType == R . P ) { N = R . N ; }//fi
    }//done
  } else { N = PType ;
  }//fi

  if ( N . isEmpty ( ) ) {
    N = ( TType . endsWith ( GPT ) ? "" : PType + "-" ) + "unknown" ;
  }//fi

  return N ;

}// PTName

#endif // PTNAME_H

//eof PTName.h
