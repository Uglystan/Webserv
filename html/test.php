<!DOCTYPE html>
<html>
<head>
    <title>Test</title>
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
</head>
<body>
    <!-- Menu de navigation -->
    <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
        <div class="container">
            <a class="navbar-brand" href="#">Test</a>
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
                        <a class="nav-link" href="test.php">Test</a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="post.php">Post</a>
                    </li>
                </ul>
            </div>
        </div>
    </nav>

    <div class="container">
        <h1>My first PHP page</h1>

        <?php
        // Vérifier si les paramètres GET 'nom' et 'prenom' existent
        if (isset($_GET['nom']) && isset($_GET['prenom'])) {
            $nom = $_GET['nom'];
            $prenom = $_GET['prenom'];
            
            echo "<p>Nom : $nom</p>";
            echo "<p>Prénom : $prenom</p>";
        } else {
            echo "Aucun nom et prénom spécifiés dans la superglobale GET.";
        }
        ?>

    </div>
</body>
</html>
