<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Réponse</title>
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
        <h1>Réponse du Formulaire</h1>
        <div class="alert alert-info">
            <?php
	    if ($_SERVER["REQUEST_METHOD"] === "POST") {
		echo "<h2>Données du formulaire POST :</h2>";
		echo "<pre>";
		var_dump($_POST);
		echo "</pre>";
	    } else {
		echo "Aucune donnée soumise via POST.";
	    }
	    ?>
        </div>
        <a href="form.html" class="btn btn-primary">Retour au Formulaire</a>
    </div>
</body>
</html>
