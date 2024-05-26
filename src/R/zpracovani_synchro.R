# Nastavení pracovního adresáře
setwd("C:/Users/kovar/Desktop/Bakalářka/BachelorThesis-Astrochronograph/src/R")

# Načtení dat z CSV souboru
data <- read.csv("synchro.csv", header = FALSE, sep = ";")

# Přejmenování sloupců na 'Time'
names(data) <- c("Time")

# Převedení sloupce 'Time' na formát POSIXct
data$Time <- as.POSIXct(data$Time, format = "%H:%M:%S %d %m %Y")

# Vytvoření nového sloupce 'Hour', který obsahuje hodinu každé synchronizace
data$Hour <- as.integer(format(data$Time, "%H"))

# Vytvoření histogramu počtu synchronizací v jednom dni
library(ggplot2)
plot <- ggplot(data, aes(x = Hour)) +
  geom_histogram(binwidth = 1, color = "black", fill = "skyblue", alpha = 0.7) +
  labs(title = "Počet synchronizací dat po hodinách (25. 4. 2024)",
       x = "Hodina (od 0 do 23)",
       y = "Počet synchronizací v dané hodině") +
  theme_minimal() +
  theme(
    plot.title = element_text(hjust = 0.5),
    panel.grid.major.x = element_blank(),  # Vypnutí svislých čar v mřížce
    panel.grid.major.y = element_line(color = "grey"),  # Vodorovné čáry v mřížce
    axis.title.x = element_text(margin = margin(t = 10))  # Posunutí popisu osy X blíže k ose
  )

# Zobrazení grafu
print(plot)

# Uložení grafu
ggsave("synchro_histogram.png", plot = plot, width = 10, height = 6, dpi = 300)
