<!DOCTYPE html>
<html>
<head>
    <title>Print env</title>
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
</head>
<body>
    <!-- Menu de navigation -->
    <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
        <div class="container">
            <a class="navbar-brand" href="#">Print Env</a>
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
        <h1>GET METHOD CONTENT:</h1>
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
        <!-- Inclure Bootstrap JS (assurez-vous de spécifier le chemin correct vers le fichier JS) -->
	<script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.3/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
