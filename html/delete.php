<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>delete file</title>
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
    <style>
        .navbar-nav .nav-item {
            transition: transform 0.2s;
        }
        .navbar-nav .nav-item:hover {
            transform: scale(1.1);
        }
    </style>
</head>
<body>
<nav class="navbar navbar-expand-lg navbar-dark bg-primary"> <!-- Utilisation de bg-primary pour la couleur bleue -->
        <div class="container">
            <a class="navbar-brand" href="#">Page D'Accueil</a>
            <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarNav">
                <ul class="navbar-nav">
                    <li class="nav-item">
                        <a class="nav-link" href="monsite.html">Accueil</a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="print.php">Print Env</a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="test.php">Test php</a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="form.html">Form</a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="post.html">Post</a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="delete.php">Delete</a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="test.py">Test .py</a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="test.sh">Test .sh</a>
                    </li>
                </ul>
            </div>
        </div>
    </nav>
    <div class="container">
        <h1>Liste des fichiers du serveur</h1>
        <?php
        $directory = 'tmp'; // Répertoire relatif
        $files = scandir($directory);
        if (count($files) > 2) { // Vérifie s'il y a des fichiers autres que '.' et '..'
            echo '<ul class="list-group">';
            foreach ($files as $file) {
                if ($file != '.' && $file != '..') {
                    echo "<li class='list-group-item d-flex justify-content-between align-items-center'>$file 
                        <a href='#' class='btn btn-delete btn-sm' onclick='deleteFile(\"$directory/$file\")'>Supprimer</a>
                    </li>";
                }
            }
            echo '</ul>';
        } else {
            echo '<p>Aucun fichier dans le serveur.</p>';
        }
        ?>
    </div>

    <script>
        function deleteFile(filePath) {
            var xhr = new XMLHttpRequest();
            xhr.open('DELETE', filePath, true);
            xhr.onreadystatechange = function () {
                if (xhr.readyState === 4) {
                    if (xhr.status === 204) {
                        alert("Le fichier a été supprimé avec succès.");
                        window.location.href = 'monsite.html'; // Redirection après la suppression réussie
                    } else {
                        alert("Erreur lors de la suppression du fichier.");
                    }
                }
            };
            xhr.send();
        }
    </script>
            <!-- Inclure Bootstrap JS (assurez-vous de spécifier le chemin correct vers le fichier JS) -->
	<script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.3/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
