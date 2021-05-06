@extends('layouts.template')
@section('title')
    Configurações
@endsection

@section('body')
    <div class="subtitle">Configurações</div>
    <form action="/config/setTime" method="get">
        <div class="container">
            <div class="row">
                <div class="col">
                    <label class="form-label" for="tempo">Ajustar o tempo de confirmação (em minutos)</label>
                    <input class="form-control" type="number" name="tempo" id="">
                </div>
                <div class="col align-self-end">
                    <button type="submit" class="btn btn-light">Salvar Configuração</button>
                </div>
            </div>
        </div>
    </form>
@endsection