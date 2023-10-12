<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>test calculette</title>
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
        <h1>Calculatrice</h1>
        
        <form action="test.php" method="post">
	<label for="nombre1">Nombre 1:</label>
	<input type="number" id="nombre1" name="nombre1" required>
	<br><br>

	<label for="operation">Opération:</label>
	<select id="operation" name="operation">
		<option value="*">*</option>
		<option value="/">/</option>
		<option value="%">%</option>
		<option value="+">+</option>
		<option value="-">-</option>
	</select>
	<br><br>
	<label for="nombre2">Nombre 2:</label>
	<input type="number" id="nombre2" name="nombre2" required>
	<br><br>

	<input type="submit" value="Calculer">
        </form>
    </div>

	<?php
		if ($_SERVER["REQUEST_METHOD"] == "POST")
		{
			$nb1 = $_POST["nombre1"];
			$nb2 = $_POST["nombre2"];
			$operator = $_POST["operation"];
			// echo "<p>$nb1</p>";
			// echo "<p>$nb2</p>";
			// echo "<p>$operator</p>";

			switch ($operator)
			{
				case "*":
					$resultat = $nb1 * $nb2;
					break;
				case "/":
					if ($nb2 != 0)
						$resultat = $nb1 / $nb2;
					else
						$resultat = "Division par 0 impossible";
					break;
				case "%":
					if ($nb2 != 0)
						$resultat = $nb1 / $nb2;
					else
						$resultat = "modulo par 0 impossible";
					break;
				case "+":
					$resultat = $nb1 + $nb2;
					break;
				case "-":
					$resultat = $nb1 - $nb2;
					break;
				default:
					$resultat = "Operation non valide";
					break;
				
			}
			echo "<h2>Résultat :</h2>";
			echo "<p>$nb1 $operator $nb2 = $resultat</p>";
		}
	?>
        <!-- Inclure Bootstrap JS (assurez-vous de spécifier le chemin correct vers le fichier JS) -->
	<script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.3/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
