<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Print env</title>
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
            <a class="navbar-brand" href="#">Print</a>
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

    <h1>Variables d'environnement PHP-CGI</h1>

    <p><strong>DOCUMENT_ROOT:</strong> <?php echo $_SERVER['DOCUMENT_ROOT']; ?></p>
    <p><strong>SCRIPT_FILENAME:</strong> <?php echo $_SERVER['SCRIPT_FILENAME']; ?></p>
    <p><strong>QUERY_STRING:</strong> <?php echo $_SERVER['QUERY_STRING']; ?></p>
    <p><strong>REQUEST_METHOD:</strong> <?php echo $_SERVER['REQUEST_METHOD']; ?></p>
    <p><strong>REMOTE_ADDR:</strong> <?php echo $_SERVER['REMOTE_ADDR']; ?></p>
    <p><strong>SERVER_NAME:</strong> <?php echo $_SERVER['SERVER_NAME']; ?></p>
    <p><strong>SERVER_PORT:</strong> <?php echo $_SERVER['SERVER_PORT']; ?></p>
    <p><strong>SERVER_PROTOCOL:</strong> <?php echo $_SERVER['SERVER_PROTOCOL']; ?></p>
    <p><strong>REQUEST_URI:</strong> <?php echo $_SERVER['REQUEST_URI']; ?></p>
    <div class="container">
        <h1>Variables si query string remplie:</h1>
        <?php
        // Vérifier si les paramètres GET 'nom' et 'prenom' existent
        if (isset($_GET['nom']) && isset($_GET['prenom'])) {
            $nom = $_GET['nom'];
            $prenom = $_GET['prenom'];
            
            echo "<p>Nom : $nom</p>";
            echo "<p>Prénom : $prenom</p>";
        } else {
            echo "Query string pas au bon format\nExemple: ... .php?nom=dupont&prenom=lol";
        }
        ?>
    </div>
    <!-- Inclure Bootstrap JS (assurez-vous de spécifier le chemin correct vers le fichier JS) -->
    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.3/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
