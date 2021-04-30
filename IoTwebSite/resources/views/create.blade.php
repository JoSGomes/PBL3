@extends('layouts.template')
@section('title')
    Cadastrar contatos
@endsection

@section('body')
<div class="subtitle">Cadastro de Contatos</div>
<form action="x" method="post">
    <div class="row g-3">
        <div class="col">
            <label for="exampleInputEmail1" class="form-label">Nome do contato</label>
            <input type="name" class="form-control" name="name"
        </div>
        <div class="col">
            <label for="exampleInputEmail1" class="form-label">Número</label>
            <input type="name" class="form-control" name="number"
        </div>
    </div>
    <br><br>
    <button type="submit" class="btn btn-outline-light me-md-2">Cadastrar</button>
</form>
@endsection