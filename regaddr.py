#!/usr/bin/python3

import re
import os
import tkinter as tk
from tkinter import filedialog, messagebox

def parse_header_and_generate_cpp(input_path):
    registers = []
    
    # Dosya adını ve yolunu ayrıştır
    directory, filename = os.path.split(input_path)
    filename_no_ext = os.path.splitext(filename)[0]
    
    # Çıktı dosyası, seçilen dosyanın yanına kaydedilir
    # Örn: iom328p.h -> reg_iom328p.hpp
    output_filename = f"reg_{filename_no_ext}.hpp"
    output_path = os.path.join(directory, output_filename)
    
    # Regex Desenleri
    # Örn: #define PORTB _SFR_IO8(0x05)
    regex_io  = re.compile(r'#define\s+(\w+)\s+_SFR_IO8\s*\((0x[0-9A-Fa-f]+)\)')
    # Örn: #define UDR0 _SFR_MEM8(0xC6)
    regex_mem = re.compile(r'#define\s+(\w+)\s+_SFR_MEM(?:8|16)\s*\((0x[0-9A-Fa-f]+)\)')

    try:
        count = 0
        with open(input_path, 'r', encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()
            
            for line in lines:
                # 1. IO Registerları Ara (Offset +0x20 eklenmeli)
                match_io = regex_io.search(line)
                if match_io:
                    name = match_io.group(1)
                    addr_str = match_io.group(2)
                    addr_int = int(addr_str, 16) + 0x20 # IO Offseti ekle!
                    registers.append((name, f"0x{addr_int:02X}"))
                    count += 1
                    continue

                # 2. MEM Registerları Ara (Direkt al)
                match_mem = regex_mem.search(line)
                if match_mem:
                    name = match_mem.group(1)
                    addr_str = match_mem.group(2)
                    addr_int = int(addr_str, 16)
                    registers.append((name, f"0x{addr_int:02X}"))
                    count += 1

        # Çıktı Dosyasını Oluştur
        with open(output_path, 'w', encoding='utf-8') as f:
            f.write("#pragma once\n")
            f.write("#include <stdint.h>\n\n")
            f.write(f"// OTOMATIK OLUSTURULDU: {filename}\n")
            f.write("// Kaynak: {}\n".format(input_path))
            f.write("namespace Addr {\n")
            
            for name, addr in registers:
                if "_vect" in name: continue # Vektörleri atla
                f.write(f"    constexpr uint16_t {name:<12} = {addr};\n")
            
            f.write("}\n")
            
        return True, f"{count} register bulundu.\nDosya oluşturuldu:\n{output_path}"

    except Exception as e:
        return False, str(e)

def main():
    # Tkinter ana penceresini oluştur ama gizle
    root = tk.Tk()
    root.withdraw() 

    # Dosya Seçme Penceresini Aç
    file_path = filedialog.askopenfilename(
        title="AVR Header Dosyasını Seçin (iom328p.h vb.)",
        filetypes=[("C Header Files", "*.h"), ("All Files", "*.*")]
    )

    if not file_path:
        print("Dosya seçilmedi, işlem iptal.")
        return

    # İşlemi Başlat
    success, message = parse_header_and_generate_cpp(file_path)

    if success:
        messagebox.showinfo("Başarılı", message)
    else:
        messagebox.showerror("Hata", f"Bir hata oluştu:\n{message}")

if __name__ == "__main__":
    main()
