import wave

# Inlocuieste cu numele fisierului tau
nume_fisier = '52.wav' 

try:
    with wave.open(nume_fisier, 'rb') as wav_file:
        channels = wav_file.getnchannels()
        framerate = wav_file.getframerate()
        n_frames = wav_file.getnframes()
        duration = n_frames / framerate
        
        print(f"--- ANALIZA FISIER: {nume_fisier} ---")
        print(f"Canale (Channels): {channels}")
        print(f"Frecventa (Sample Rate): {framerate} Hz")
        print(f"Cadre audio (Frames): {n_frames}")
        print(f"Durata exacta: {duration:.4f} secunde")
        
        total_samples_txt = n_frames * channels
        print(f"\nNumarul total de valori in vectorul tau ar trebui sa fie: {total_samples_txt}")
        
        if channels == 2:
            print("(Deoarece e Stereo, scriptul tau de conversie pune ambele canale in vector,")
            print("deci ai de 2 ori mai multe puncte decat ai calculat initial.)")

except FileNotFoundError:
    print("Nu am gasit fisierul. Verifica numele.")