#!/usr/bin/php-cgi
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Résultat du Formulaire</title>
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
        <h1>Résultat du Formulaire</h1>

        <?php
        if ($_SERVER['REQUEST_METHOD'] === 'POST') {
            $nom = isset($_POST['nom']) ? $_POST['nom'] : '';
            $prenom = isset($_POST['prenom']) ? $_POST['prenom'] : '';
            $telephone = isset($_POST['telephone']) ? $_POST['telephone'] : '';

            echo "<p>Nom : $nom</p>";
            echo "<p>Prénom : $prenom</p>";
            echo "<p>Téléphone : $telephone</p>";
        } else {
            echo "Aucune donnée de formulaire reçue.";
        }
        ?>
        <p><a href="form.html">Retour au formulaire</a></p>
    </div>
</body>
</html>
