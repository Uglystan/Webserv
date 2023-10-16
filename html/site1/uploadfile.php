#!/usr/bin/php
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Téléchargement Réussi</title>
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            background-color: #f4f4f4;
        }
        .container {
            margin: 100px auto;
            padding: 20px;
            max-width: 600px;
            background-color: #fff;
            border-radius: 5px;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2);
        }
        h1 {
            color: #007bff; /* Bleu Bootstrap */
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Téléchargement Réussi</h1>
	<?php
if (isset($_FILES["fichier"]) && $_FILES["fichier"]["error"] == 0)
{
    $nomFichier = $_FILES["fichier"]["name"];
    $nomFichier = str_replace(' ', '%20', $nomFichier);
    $dossierDeDestination = "tmp/"; // Le dossier où vous souhaitez stocker les fichiers téléchargés
    
    // Déplacer le fichier téléchargé vers le dossier de destination
    if (move_uploaded_file($_FILES["fichier"]["tmp_name"], $dossierDeDestination . $nomFichier))
    {
        echo "<p>Le fichier $nomFichier a été téléchargé avec succès.</p>";
        
        // Obtenez le type MIME du fichier
        $typeMIME = mime_content_type($dossierDeDestination . $nomFichier);
        
        $style = 'width:100%; height:auto;';

        if (strpos($typeMIME, "image/") === 0)
        {
            // Afficher une image si c'est une image
            echo '<p>Voici l\'image :</p>';
            echo '<img src="' . $dossierDeDestination . $nomFichier . '" alt="Image" style="' . $style . '" />';
        }
        elseif ($typeMIME == "application/pdf")
        {
            // Afficher un PDF si c'est un PDF
            echo '<p>Voici le fichier PDF :</p>';
            echo '<embed src="' . $dossierDeDestination . $nomFichier . '" type="application/pdf" style="' . $style . '" />';
        }
        elseif (strpos($typeMIME, "audio/") === 0)
        {
            // Afficher un audio si c'est un fichier audio
            echo '<p>Voici le fichier audio :</p>';
            echo '<audio controls style="' . $style . '">';
            echo '<source src="' . $dossierDeDestination . $nomFichier . '" type="' . $typeMIME . '">';
            echo 'Votre navigateur ne prend pas en charge l\'élément audio.';
            echo '</audio>';
        }
        elseif (strpos($typeMIME, "text/") === 0)
        {
            // Afficher le texte si c'est un fichier texte
            echo '<p>Contenu du fichier texte :</p>';
            echo '<pre>';
            echo file_get_contents($dossierDeDestination . $nomFichier);
            echo '</pre>';
        }
        else
        {
            echo '<p>Le fichier n\'est pas pris en charge.</p>';
        }
    }
    else
    {
        echo "<p>Une erreur s'est produite lors du téléchargement du fichier.</p>";
    }
}
else
{
    echo "<p>Aucun fichier n'a été téléchargé ou une erreur s'est produite.</p>";
}
?>


        <p><a href="post.html">Retour au formulaire</a></p>
    </div>
    
    <!-- Inclure Bootstrap JS (assurez-vous de spécifier le chemin correct vers le fichier JS) -->
    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.3/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
