# Audio Signal Analyzer (LabWindows/CVI)

Aplicație software pentru analiza și prelucrarea semnalelor audio
în domeniul timp și frecvență, dezvoltată în LabWindows/CVI (ANSI C).

## Tehnologii folosite
- LabWindows/CVI (ANSI C)
- Analysis Library (FFT, filtre, statistici)
- User Interface Library (GUI)

## Funcționalități

### Domeniul Timp
- Încărcarea și vizualizarea fișierelor audio `.wav`
- Calcul parametri statistici: Min, Max, Mean, Median,
  Variance, Skewness, Kurtosis
- Filtru de Mediere (fereastră N=32)
- Filtru de Ordinul I / Alpha (trece-jos recursiv)
- Afișare histogramă a nivelurilor semnalului

### Domeniul Frecvență
- Calcul spectru de putere în timp real prin FFT (ferestre de 1024 puncte)
- Funcții de ferestruire: Rectangle și Hanning
- Detecție automată a frecvenței dominante (Peak Frequency)
- Salvare automată a graficelor în format JPEG

## Documentație
Vezi fișierul `Documentatie.pdf` pentru detalii complete
de implementare și capturi de ecran.
