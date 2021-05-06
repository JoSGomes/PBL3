@extends('layouts.template')

@section('title')
    Home
@endsection

@section('body')
    <div class="subtitle">Dispositivo [ESTADO]</div>
    <div class="subtitle">Contatos de emergência</div>
    <div class="container">
        <div class="row">
            <table class="table table-hover table-bordeless">
                <thead>
                <tr>
                    <th scope="col">Nome</th>
                    <th scope="col">Número</th>
                </tr>
                </thead>
                <tbody>
                <tr >
                    <td>Mark</td>
                    <td>Otto</td>
                </tr>
                </tbody>
            </table>
        </div>
    </div>
@endsection